use std::collections::{HashMap, hash_map};

use futures_util::StreamExt;

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

struct ScreenSaver {
    policy_agent: PolicyAgentProxy<'static>,
    known_names: HashMap<zbus::names::UniqueName<'static>, Vec<u32>>
}

#[zbus::interface(name = "org.freedesktop.ScreenSaver")]
impl ScreenSaver {
    async fn inhibit(
        &mut self, name: &str, reason: &str,
        #[zbus(header)] header: zbus::message::Header<'_>,
    ) -> zbus::fdo::Result<u32> {
        let cookie = self.policy_agent.add_inhibition(CHANGE_SCREEN_SETTINGS, name, reason).await?;

        match self.known_names.entry(header.sender().unwrap().to_owned()) {
            hash_map::Entry::Vacant  (x) => {x.insert(vec![cookie]);},
            hash_map::Entry::Occupied(mut x) => x.get_mut().push(cookie),
        };

        Ok(cookie)
    }

    async fn uninhibit(&mut self, cookie: u32) -> zbus::fdo::Result<()> {
        Ok(self.policy_agent.release_inhibition(cookie).await?)
    }
}

async fn handle_event(
    conn: &zbus::Connection,
    policy_agent: &PolicyAgentProxy<'static>,
    event: zbus::fdo::NameOwnerChanged
) -> anyhow::Result<()> {
    let args = event.args()?;
    let None = *args.new_owner else {return Ok(())};

    let screensaver = conn.object_server()
        .interface::<_, ScreenSaver>("/org/freedesktop/ScreenSaver").await?;
    let mut screensaver = screensaver.get_mut().await;

    let Ok(unique_name) = zbus::names::UniqueName::try_from(args.name) else {return Ok(())};

    let Some(cookies) = screensaver.known_names.remove(unique_name.as_str()) else {return Ok(())};

    for cookie in cookies {
        policy_agent.release_inhibition(cookie).await?;
    }
    Ok(())
}

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    let conn = zbus::connection::Builder::session()?
        .name("org.freedesktop.ScreenSaver")?
        .build().await?;

    let policy_agent = PolicyAgentProxy::new(&conn).await?;

    let screensaver = ScreenSaver {
        policy_agent: policy_agent.clone(),
        known_names: HashMap::new()
    };
    conn.object_server()
        .at("/org/freedesktop/ScreenSaver", screensaver).await?;

    let dbus = zbus::fdo::DBusProxy::new(&conn).await?;

    let mut stream = dbus.receive_name_owner_changed().await?;

    while let Some(event) = stream.next().await {
        if let Err(e) = handle_event(&conn, &policy_agent, event).await {
            println!("Error while handling NameOwnerChanged: {e}")
        }
    }

    Ok(())
}
