#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <wayland-client-protocol.h>
#include <wayland-server-core.h>
#include <wayland-server.h>
#include <wayland-util.h>
#include "plasma_shell.h"
#include "log.h"
#include "plasma-shell-protocol.h"
#include "sway/input/input-manager.h"
#include "sway/input/seat.h"
#include "sway/tree/container.h"
#include "sway/tree/view.h"
#include "sway/tree/workspace.h"

#define PLASMA_SHELL_VERSION 8

static const struct org_kde_plasma_shell_interface plasma_shell_implementation;
static const struct org_kde_plasma_surface_interface plasma_surface_implementation;

static struct plasma_surface *plasma_surface_from_resource(struct wl_resource *resource) {
	assert(wl_resource_instance_of(
		resource,
		&org_kde_plasma_surface_interface,
		&plasma_surface_implementation
	));
	return wl_resource_get_user_data(resource);
}

static void plasma_surface_resource_destroy(struct wl_resource *resource) {
	struct plasma_surface *surface = plasma_surface_from_resource(resource);

	if (surface->surface != NULL) {
		wl_list_remove(&surface->link);
		wl_list_remove(&surface->parent_destroy.link);
	}
	free(surface);
}

static void plasma_surface_parent_destroy(struct wl_listener *listener, void *data) {
	struct plasma_surface *surface = wl_container_of(
		listener, surface, parent_destroy
	);

	// We consider this surface invalid, and remove it from the lookup list
	wl_list_remove(&surface->link);
	wl_list_remove(&surface->parent_destroy.link);
	surface->surface = NULL;

	// The XML spec says that the resource should be destroyed, but KWin doesn't do that.
	// Doing so causes plasmashell crashes
	// wl_resource_destroy(surface->resource);
}

static void plasma_surface_destroy(struct wl_client *client, struct wl_resource *resource) {
	wl_resource_destroy(resource);
}

static void plasma_surface_set_output(
	struct wl_client *client,
	struct wl_resource *resource,
	struct wl_resource *output
) {
	struct plasma_surface *surface = plasma_surface_from_resource(resource);
	if (!surface->surface) return;
	sway_log(SWAY_INFO, "STUB: plasma_surface_set_output(%p, _)", surface);
}

bool plasma_surface_apply_position(
	struct plasma_surface *surface,
	struct sway_container *container
) {
	bool updated = false;

	struct sway_seat *seat = input_manager_current_seat();
	struct sway_workspace *workspace = seat_get_focused_workspace(seat);

	if (!workspace) return updated;

	// https://github.com/KDE/kwin/blob/38faf84c3b25a90ca3eb3f5f18476c048a9ff877/src/placement.cpp#L34-L56
	switch (surface->role) {
		case ORG_KDE_PLASMA_SURFACE_ROLE_ONSCREENDISPLAY:
		case ORG_KDE_PLASMA_SURFACE_ROLE_NOTIFICATION:
		case ORG_KDE_PLASMA_SURFACE_ROLE_CRITICALNOTIFICATION:
			container_set_floating(container, true);
			sway_log(SWAY_DEBUG, "Move plasma surface %p to lower-center", surface);
			container_floating_move_to(
				container,
				(double)workspace->width / 2 - container->pending.width / 2,
				2 * (double)workspace->height / 3 - container->pending.height / 2
			);
			updated = true;
			break;

		default:
			break;
	}

	if (surface->position_set) {
		container_set_floating(container, true);
		sway_log(
			SWAY_DEBUG, "Move plasma surface %p to %d, %d",
			surface, surface->x, surface->y
		);
		container_floating_move_to(container, surface->x, surface->y);
		updated = true;
	}
	return updated;
}

bool plasma_surface_can_take_focus(struct plasma_surface *surface) {
	switch (surface->role) {
		case ORG_KDE_PLASMA_SURFACE_ROLE_NORMAL:
		case ORG_KDE_PLASMA_SURFACE_ROLE_APPLETPOPUP:
			return true;
		default:
			return false;
	}
}

static void plasma_surface_set_position(
	struct wl_client *client,
	struct wl_resource *resource,
	int32_t x,
	int32_t y
) {
	struct plasma_surface *surface = plasma_surface_from_resource(resource);
	if (!surface->surface) return;

	surface->position_set = true;
	surface->x = x;
	surface->y = y;

	struct sway_view *view = view_from_wlr_surface(surface->surface);
	if (!view || !view->container) return;
	plasma_surface_apply_position(surface, view->container);
}

static void plasma_surface_set_role(
	struct wl_client *client,
	struct wl_resource *resource,
	uint32_t role
) {
	struct plasma_surface *surface = plasma_surface_from_resource(resource);
	if (!surface->surface) return;

	surface->role = role;

	struct sway_view *view = view_from_wlr_surface(surface->surface);
	if (!view || !view->container) return;
	plasma_surface_apply_position(surface, view->container);
}

static void plasma_surface_set_panel_behavior(
	struct wl_client *client,
	struct wl_resource *resource,
	uint32_t flag
) {
	struct plasma_surface *surface = plasma_surface_from_resource(resource);
	if (!surface->surface) return;
	sway_log(SWAY_INFO, "STUB: plasma_surface_set_panel_behavior(%p, %d)", surface, flag);
}

static void plasma_surface_set_skip_taskbar(
	struct wl_client *client,
	struct wl_resource *resource,
	uint32_t skip
) {
	struct plasma_surface *surface = plasma_surface_from_resource(resource);
	if (!surface->surface) return;
	sway_log(SWAY_INFO, "STUB: plasma_surface_set_skip_taskbar(%p, %d)", surface, skip);
}

static void plasma_surface_panel_auto_hide_hide(
	struct wl_client *client,
	struct wl_resource *resource
) {
	struct plasma_surface *surface = plasma_surface_from_resource(resource);
	if (!surface->surface) return;
	sway_log(SWAY_INFO, "STUB: plasma_surface_panel_auto_hide_hide(%p)", surface);
}

static void plasma_surface_panel_auto_hide_show(
	struct wl_client *client,
	struct wl_resource *resource
) {
	struct plasma_surface *surface = plasma_surface_from_resource(resource);
	if (!surface->surface) return;
	sway_log(SWAY_INFO, "STUB: plasma_surface_panel_auto_hide_show(%p)", surface);
}

static void plasma_surface_set_panel_takes_focus(
	struct wl_client *client,
	struct wl_resource *resource,
	uint32_t takes_focus
) {
	struct plasma_surface *surface = plasma_surface_from_resource(resource);
	if (!surface->surface) return;
	sway_log(SWAY_INFO, "STUB: plasma_surface_set_panel_takes_focus(%p, %d)", surface, takes_focus);
}

static void plasma_surface_set_skip_switcher(
	struct wl_client *client,
	struct wl_resource *resource,
	uint32_t skip
) {
	struct plasma_surface *surface = plasma_surface_from_resource(resource);
	if (!surface->surface) return;
	sway_log(SWAY_INFO, "STUB: plasma_surface_set_skip_switcher(%p, %d)", surface, skip);
}

static void plasma_surface_open_under_cursor(
	struct wl_client *client,
	struct wl_resource *resource
) {
	struct plasma_surface *surface = plasma_surface_from_resource(resource);
	if (!surface->surface) return;
	sway_log(SWAY_INFO, "STUB: plasma_surface_open_under_cursor(%p)", surface);
}

static const struct org_kde_plasma_surface_interface plasma_surface_implementation = {
	.destroy               = plasma_surface_destroy,
	.set_output            = plasma_surface_set_output,
	.set_position          = plasma_surface_set_position,
	.set_role              = plasma_surface_set_role,
	.set_panel_behavior    = plasma_surface_set_panel_behavior,
	.set_skip_taskbar      = plasma_surface_set_skip_taskbar,
	.panel_auto_hide_hide  = plasma_surface_panel_auto_hide_hide,
	.panel_auto_hide_show  = plasma_surface_panel_auto_hide_show ,
	.set_panel_takes_focus = plasma_surface_set_panel_takes_focus,
	.set_skip_switcher     = plasma_surface_set_skip_switcher    ,
	.open_under_cursor     = plasma_surface_open_under_cursor    
};

static struct plasma_shell *plasma_shell_from_resource(struct wl_resource *resource) {
	assert(wl_resource_instance_of(
		resource,
		&org_kde_plasma_shell_interface,
		&plasma_shell_implementation
	));
	return wl_resource_get_user_data(resource);
}

static void plasma_shell_get_surface(
		struct wl_client *wl_client,
		struct wl_resource *client_resource,
		uint32_t id,
		struct wl_resource *surface_resource
) {
	struct plasma_shell *shell = plasma_shell_from_resource(client_resource);
	struct wlr_surface *wlr_surface = wlr_surface_from_resource(surface_resource);

	struct plasma_surface *surface = calloc(1, sizeof(*surface));
	if (surface == NULL) {
		goto error;
	}

	surface->shell = shell;
	surface->surface = wlr_surface;

	surface->resource = wl_resource_create(
		wl_client,
		&org_kde_plasma_surface_interface,
		wl_resource_get_version(client_resource),
		id
	);
	if (surface->resource == NULL) {
		goto error_surface;
	}

	wl_list_insert(&shell->surfaces, &surface->link);

	wl_resource_set_implementation(
		surface->resource,
		&plasma_surface_implementation,
		surface,
		plasma_surface_resource_destroy
	);

	surface->parent_destroy.notify = plasma_surface_parent_destroy;
	wl_signal_add(&wlr_surface->events.destroy, &surface->parent_destroy);

	return;

error_surface:
	free(surface);
error:
	wl_client_post_no_memory(wl_client);
}

static const struct org_kde_plasma_shell_interface plasma_shell_implementation = {
	.get_surface = plasma_shell_get_surface
};

static void plasma_shell_bind(struct wl_client *wl_client, void *data, uint32_t version, uint32_t id) {
	struct plasma_shell *plasma_shell = data;

	struct wl_resource *resource = wl_resource_create(
		wl_client, &org_kde_plasma_shell_interface, version, id
	);
	if (resource == NULL) {
		wl_client_post_no_memory(wl_client);
		return;
	}
	wl_resource_set_implementation(
		resource,
		&plasma_shell_implementation,
		plasma_shell,
		NULL
	);
}

static void plasma_shell_parent_destroy(struct wl_listener *listener, void *data) {
	struct plasma_shell *plasma_shell = wl_container_of(
		listener, plasma_shell, parent_destroy
	);

	wl_list_remove(&plasma_shell->parent_destroy.link);
	wl_global_destroy(plasma_shell->global);
	free(plasma_shell);
}

struct plasma_shell *plasma_shell_create(struct wl_display *display, uint32_t version) {
	assert(version <= PLASMA_SHELL_VERSION);

	struct plasma_shell *plasma_shell = calloc(1, sizeof(*plasma_shell));
	if (!plasma_shell) {return NULL;}

	wl_list_init(&plasma_shell->surfaces);

	struct wl_global *global = wl_global_create(
		display,
		&org_kde_plasma_shell_interface,
		version,
		plasma_shell,
		plasma_shell_bind
	);
	if (!global) {
		free(plasma_shell);
		return NULL;
	}
	plasma_shell->global = global;

	plasma_shell->parent_destroy.notify = plasma_shell_parent_destroy;
	wl_display_add_destroy_listener(display, &plasma_shell->parent_destroy);

	return plasma_shell;
}

struct plasma_surface *plasma_shell_find_plasma_surface(
	struct plasma_shell *shell,
	struct wlr_surface *surface
) {
	struct plasma_surface *ptr;
	wl_list_for_each(ptr, &shell->surfaces, link) {
		if (ptr->surface == surface) {return ptr;}
	}
	return NULL;
}
