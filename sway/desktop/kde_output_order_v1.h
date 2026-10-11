#include "kde-output-order-v1-protocol.h"
#include "sway/tree/root.h"
#include <wayland-server-core.h>

struct kde_output_order_v1 {
	struct wl_global *global;
	struct wl_list client_resources;

	struct wl_listener parent_destroy;
};

struct kde_output_order_v1 *kde_output_order_v1_create(struct wl_display *display);

void kde_output_order_v1_update_outputs(struct kde_output_order_v1 *output_order);
