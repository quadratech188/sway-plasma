sway-plasma(1)

# NAME

sway-plasma - Sway with some Plasma protocols

# SYNOPSIS

*sway-plasma* [options...] [command]

# OPTIONS

*-h, --help*
	Show help message and quit.

*-c, --config* <config>
	Specifies a config file.

*-C, --validate*
	Check the validity of the config file, then exit.

*-d, --debug*
	Enables full logging, including debug information.

*-v, --version*
	Show the version number and quit.

*-V, --verbose*
	Enables more verbose logging.

*--get-socketpath*
	Gets the IPC socket path and prints it, then exits.

# DESCRIPTION

sway-plasma was created to fill the need of an sway-like window manager for
KDE Plasma. The upstream sway developers (understandably) have no intention of
adding Plasma protocols to sway. Many thanks to the sway folks for providing such
a great piece of software, so good that your users would rather figure out how
to shoehorn it into a Desktop Environment that wasn't built for this kind of
thing, rather than switching to something else.

You can run sway-plasma directly from a tty, or via a Wayland-compatible login manager.

# CONFIGURATION

sway-plasma searches for a config file in the following locations, in this order:

. ~/.sway-plasma/config
. $XDG_CONFIG_HOME/sway-plasma/config (suggested location)
. /etc/sway-plasma/config

If unset, $XDG_CONFIG_HOME defaults to *~/.config*.

An error is raised when no config file is found. The recommended default
configuration is usually installed to */etc/sway-plasma/config*; you are encouraged
to copy this to *~/.config/sway-plasma/config* and edit it from there.

For information on the config file format, see *sway*(5).

# IPC COMMANDS

Though *swaymsg*(1) is generally preferred, you may run *sway-plasma* _command_ to
send _command_ to the running instance of sway. You can also issue commands
with *i3-msg*(1) or even with *i3*(1).

# ENVIRONMENT

The following environment variables have an effect on sway-plasma:

_SWAYSOCK_
	Specifies the path to the sway-plasma IPC socket.

_XKB\_DEFAULT\_RULES_, _XKB\_DEFAULT\_MODEL_, _XKB\_DEFAULT\_LAYOUT_,
_XKB\_DEFAULT\_VARIANT_, _XKB\_DEFAULT\_OPTIONS_
	Configures the xkb keyboard settings. See *xkeyboard-config*(7). The
	preferred way to configure the keyboard is via the configuration file, see
	*sway-input*(5).

The following environment variables are set by sway-plasma:

_DISPLAY_
	If compiled with Xwayland support and Xwayland is not disabled by the
	config, this will be set to the name of the X display used for Xwayland.

_I3SOCK_
	For compatibility with i3, specifies the path to the sway-plasma IPC socket.

_SWAYSOCK_
	Specifies the path to the sway-plasma IPC socket.

_WAYLAND_DISPLAY_
	Specifies the name of the Wayland display that sway-plasma is running on.

_XCURSOR_SIZE_
	Specifies the configured cursor size.

_XCURSOR_THEME_
	Specifies the configured cursor theme.

# AUTHORS

Developed at https://github.com/quadratech188/sway-plasma
by quadratech188 <quadratech188@gmail.com

(Original credits)
Maintained by Simon Ser <contact@emersion.fr>, who is assisted by other open
source contributors. For more information about sway development, see
<https://github.com/swaywm/sway>.

# SEE ALSO

*sway*(5) *swaymsg*(1) *sway-input*(5) *sway-output*(5) *sway-bar*(5)
*sway-ipc*(7)
