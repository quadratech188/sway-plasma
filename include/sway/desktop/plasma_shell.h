#ifndef _PLASMA_SHELL_H
#define _PLASMA_SHELL_H

#include "plasma-shell-protocol.h"
#include "sway/tree/container.h"
#include "sway/tree/view.h"
#include <wayland-server-core.h>

struct plasma_shell {
	struct wl_global *global;

	struct wl_list unassigned_surfaces;

	struct wl_listener parent_destroy;
};

struct plasma_shell *plasma_shell_create(struct wl_display *display, uint32_t version);

struct plasma_surface *plasma_shell_find_plasma_surface(
	struct plasma_shell *shell,
	struct wlr_surface *surface
);

struct plasma_surface {
	struct sway_view *view;

	struct {
		enum org_kde_plasma_surface_role role;
		struct {
			int32_t x, y;
			bool configured;
		} position;
		enum {
			TAKES_FOCUS_DEFAULT = 0,
			TAKES_FOCUS_OVERRIDE_TRUE,
			TAKES_FOCUS_OVERRIDE_FALSE
		} takes_focus;
	} config;

	// Only used for lookups.
	struct wlr_surface *surface;

	struct plasma_shell *shell;
	struct wl_list unassigned_surfaces_link;
	struct wl_listener parent_destroy;
};

void plasma_surface_attach_to_view(struct plasma_surface *plasma, struct sway_view *view);
void plasma_surface_detach_from_view(struct plasma_surface *plasma);

bool plasma_surface_takes_focus(struct plasma_surface *plasma);
bool plasma_surface_allows_tiling(struct plasma_surface *plasma);

void plasma_surface_apply_container_attributes(struct plasma_surface *plasma);
#endif
