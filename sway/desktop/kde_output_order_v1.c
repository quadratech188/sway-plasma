#include "kde_output_order_v1.h"
#include "kde-output-order-v1-protocol.h"
#include "sway/output.h"
#include <stdlib.h>
#include <wayland-server-core.h>
#include <wayland-server.h>
#include <wayland-util.h>

#define KDE_OUTPUT_ORDER_V1_VERSION 1

static void update_outputs(struct wl_resource *resource) {
	// Ignore disabled outputs
	for (int i = 0; i < root->outputs->length; ++i) {
		struct sway_output *output = root->outputs->items[i];
		kde_output_order_v1_send_output(resource, output->wlr_output->name);
	}
	kde_output_order_v1_send_done(resource);
}

void kde_output_order_v1_update_outputs(struct kde_output_order_v1 *output_order) {
	struct wl_resource *resource;

	wl_resource_for_each(resource, &output_order->client_resources) {
		update_outputs(resource);
	}
}

static void output_order_destroy(struct wl_client *client, struct wl_resource *resource) {
	wl_resource_destroy(resource);
}

static void output_order_resource_destroy(struct wl_resource *resource) {
	wl_list_remove(wl_resource_get_link(resource));
}

static const struct kde_output_order_v1_interface output_order_implementation = {
	.destroy = output_order_destroy
};

static void output_order_bind(
	struct wl_client *wl_client, void *data, uint32_t version, uint32_t id
) {
	struct kde_output_order_v1 *output_order = data;

	struct wl_resource *resource = wl_resource_create(
		wl_client, &kde_output_order_v1_interface, version, id
	);
	if (resource == NULL) {
		wl_client_post_no_memory(wl_client);
		return;
	}
	wl_resource_set_implementation(
		resource,
		&output_order_implementation,
		output_order,
		output_order_resource_destroy
	);
	wl_list_insert(&output_order->client_resources, wl_resource_get_link(resource));

	// The compositor will send the list of outputs when the global is bound...
	update_outputs(resource);
}

static void output_order_parent_destroy(struct wl_listener *listener, void *data) {
	struct kde_output_order_v1 *output_order = wl_container_of(
		listener, output_order, parent_destroy
	);

	wl_list_remove(&output_order->parent_destroy.link);
	wl_global_destroy(output_order->global);

	struct wl_resource *resource, *tmp;
	wl_resource_for_each_safe(resource, tmp, &output_order->client_resources) {
		wl_resource_destroy(resource);
	}

	free(output_order);
}

struct kde_output_order_v1 *kde_output_order_v1_create(struct wl_display *display) {
	struct kde_output_order_v1 *output_order = calloc(1, sizeof(*output_order));
	if (!output_order) {return NULL;}

	struct wl_global *global = wl_global_create(
		display,
		&kde_output_order_v1_interface,
		KDE_OUTPUT_ORDER_V1_VERSION,
		output_order,
		output_order_bind
	);
	if (!global) {
		free(output_order);
		return NULL;
	}
	output_order->global = global;

	wl_list_init(&output_order->client_resources);

	output_order->parent_destroy.notify = output_order_parent_destroy;
	wl_display_add_destroy_listener(display, &output_order->parent_destroy);

	return output_order;
}
