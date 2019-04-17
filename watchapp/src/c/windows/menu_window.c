#include "menu_window.h"
#include "countdown_window.h"

/* static Window *window; */
SimpleMenuLayer *menu_layer;

SimpleMenuItem menu_items[] = {
	{ "A7", "68 kg+", NULL, on_menu_selected },
	{ "B1", "112 kg.", NULL, on_menu_selected },
	{ "B3", "114 kg+", NULL, on_menu_selected },
	{ "F2.1", "64 kg.", NULL, on_menu_selected },
	{ "C1", "112 kg", NULL, on_menu_selected },
	{ "C3", NULL, NULL, on_menu_selected },
	{ "D7", NULL, NULL, on_menu_selected },
	{ "E2", NULL, NULL, on_menu_selected }
};

SimpleMenuSection sections[1];


static void on_menu_selected(int index, void *ctx) {
	countdown_window_push((const char*)menu_items[index].title);
}

static void window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);


	sections[0] = (SimpleMenuSection) {
		.title =  "Maschinen",
		.items = menu_items,
		.num_items = sizeof(menu_items) / sizeof(SimpleMenuItem)
	};

	menu_layer = simple_menu_layer_create(bounds, window, sections, 1, NULL);

	layer_add_child(window_layer, simple_menu_layer_get_layer(menu_layer));
}

static void window_unload(Window *window) {
	window_destroy(window);
}

void menu_window_push() {
	Window *window;

	window = window_create();
	window_set_window_handlers(window, (WindowHandlers) {
			.load = window_load,
			.unload = window_unload
	});
	
	window_stack_push(window, true);
}

