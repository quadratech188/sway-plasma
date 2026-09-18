# `sway-plasma`

## What's here
- A partial implementation of KDE's [plasma-shell](https://wayland.app/protocols/kde-plasma-shell) wayland protocol in sway

- Some other stuff in `etc/`

## Usage
Very unfriendly as of now.  

1. Compile the sway fork.
2. `sed -i1 's|/home/quadratech/Projects/sway-plasma|<REPO LOCATION>|g' etc/*`
3. Create a new sway config, and in it source `./etc/sway-config`
4. Install `./etc/plasma-kwin_wayland.service` as a systemd user unit.
5. Select 'Plasma' from your display manager.
