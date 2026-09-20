use std::{collections::{HashMap, HashSet, hash_map}, sync::Arc};

use futures_util::StreamExt;
use tokio::sync::Mutex;

#[zbus::proxy(
    interface       = "org.kde.Solid.PowerManagement.PolicyAgent",
    default_service = "org.kde.Solid.PowerManagement.PolicyAgent",
    default_path    = "/org/kde/Solid/PowerManagement/PolicyAgent",
)]
trait PolicyAgent {
    fn add_inhibition(&self, types: u32, app_name: &str, reason: &str) -> zbus::Result<u32>;
    fn release_inhibition(&self, cookie: u32) -> zbus::Result<()>;
}

const CHANGE_SCREEN_SETTINGS: u32 = 4;

struct ScreenSaverState {
    policy_agent: PolicyAgentProxy<'static>,
    known_names: HashMap<zbus::names::UniqueName<'static>, HashSet<u32>>
}

struct ScreenSaver {
    state: Arc<Mutex<ScreenSaverState>>
}

#[zbus::interface(name = "org.freedesktop.ScreenSaver")]
impl ScreenSaver {
    async fn inhibit(
        &self, name: &str, reason: &str,
        #[zbus(header)] header: zbus::message::Header<'_>,
    ) -> zbus::fdo::Result<u32> {
        let sender = header.sender().unwrap().to_owned();

        let mut this = self.state.lock().await;
        println!("Inhibit by {sender}: {name}, {reason}");
        let cookie = this.policy_agent.add_inhibition(CHANGE_SCREEN_SETTINGS, name, reason).await?;
        println!("Inhibit by {sender}: {name}, {reason} -> {cookie}");

        this.known_names.entry(header.sender().unwrap().to_owned())
            .or_default().insert(cookie);
        Ok(cookie)
    }

    async fn un_inhibit(
        &self, cookie: u32,
        #[zbus(header)] header: zbus::message::Header<'_>,
    ) -> zbus::fdo::Result<()> {
        let sender = header.sender().unwrap().to_owned();

        let mut this = self.state.lock().await;
        println!("UnInhibit by {sender}: {cookie}");

        let result = match this.known_names.entry(sender.clone()) {
            hash_map::Entry::Vacant(_) => false,
            hash_map::Entry::Occupied(mut x) => {
                let result = x.get_mut().remove(&cookie);
                if x.get().is_empty() {x.remove();}
                result
            }
        };
        if !result {
            println!("UnInhibit by {sender}:  {cookie} -> Cookie not associated with sender");
            return Err(zbus::fdo::Error::InvalidArgs(
                "Cookie not associated with sender".into(),
            ));
        }

        this.policy_agent.release_inhibition(cookie).await?;
        println!("UnInhibit by {sender}: {cookie} -> Ok");
        Ok(())
    }
}

async fn handle_event(
    screensaver: &Arc<Mutex<ScreenSaverState>>,
    event: zbus::fdo::NameOwnerChanged
) -> anyhow::Result<()> {
    let args = event.args()?;
    let None = *args.new_owner else {return Ok(())};
    let Ok(unique_name) = zbus::names::UniqueName::try_from(args.name) else {return Ok(())};

    let mut this = screensaver.lock().await;
    let Some(cookies) = this.known_names.remove(unique_name.as_str()) else {return Ok(())};

    println!("Client {unique_name} died: UnInhibiting cookies");
    for cookie in cookies {
        this.policy_agent.release_inhibition(cookie).await?;
    }
    println!("Client {unique_name} died: UnInhibited cookies");
    Ok(())
}

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    let conn = zbus::connection::Builder::session()?
        .name("org.freedesktop.ScreenSaver")?
        .build().await?;

    let policy_agent = PolicyAgentProxy::new(&conn).await?;

    let state = Arc::new(Mutex::new(ScreenSaverState {
        policy_agent: policy_agent,
        known_names: HashMap::new()
    }));

    conn.object_server()
        .at("/org/freedesktop/ScreenSaver", ScreenSaver { state: state.clone() }).await?;
    conn.object_server()
        .at("/ScreenSaver", ScreenSaver { state: state.clone() }).await?;

    let dbus = zbus::fdo::DBusProxy::new(&conn).await?;

    let mut stream = dbus.receive_name_owner_changed().await?;

    while let Some(event) = stream.next().await {
        if let Err(e) = handle_event(&state, event).await {
            println!("Error while handling NameOwnerChanged: {e}")
        }
    }

    Ok(())
}
