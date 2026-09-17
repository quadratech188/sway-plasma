#include <wayland-server-core.h>

struct plasma_shell {
	struct wl_global *global;

	struct wl_listener parent_destroy;
};

struct plasma_shell *plasma_shell_create(struct wl_display *display, uint32_t version);

struct plasma_surface {
	struct plasma_shell *shell;
	struct wlr_surface *surface;
	struct wl_resource *resource;

	struct wl_listener parent_destroy;
};
