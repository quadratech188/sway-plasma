#include "plasma-shell-protocol.h"
#include "sway/tree/container.h"
#include <wayland-server-core.h>

struct plasma_shell {
	struct wl_global *global;

	struct wl_list surfaces;

	struct wl_listener parent_destroy;
};

struct plasma_shell *plasma_shell_create(struct wl_display *display, uint32_t version);

struct plasma_surface {
	struct wl_list link;

	struct plasma_shell *shell;
	struct wlr_surface *surface;
	struct wl_resource *resource;

	enum org_kde_plasma_surface_role role;

	int32_t x;
	int32_t y;
	bool position_set;

	struct wl_listener parent_destroy;
};

struct plasma_surface *plasma_shell_find_plasma_surface(
	struct plasma_shell *shell,
	struct wlr_surface *surface
);

bool plasma_surface_apply_position(
	struct plasma_surface *surface,
	struct sway_container *container
);

bool plasma_surface_can_take_focus(struct plasma_surface *surface);
