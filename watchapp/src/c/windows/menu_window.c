#include "../kiesl.h"
#include "menu_window.h"
#include "countdown_window.h"

/* static Window *window; */
MenuLayer *menu_layer;

SimpleMenuItem menu_items[] = {
	{ "A7", "68 kg+", NULL, NULL },
	{ "B1", "112 kg.", NULL, NULL },
	{ "B3", "114 kg+", NULL, NULL },
	{ "F2.1", "64 kg.", NULL, NULL },
	{ "C1", "112 kg", NULL, NULL },
	{ "C3", NULL, NULL, NULL },
	{ "D7", NULL, NULL, NULL },
	{ "E2", NULL, NULL, NULL }
};

#define NUM_SECTIONS 1
#define NUM_ITEMS 8


static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
	return NUM_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
	return NUM_ITEMS;
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
	return MENU_CELL_BASIC_HEADER_HEIGHT;
}


static void menu_draw_header_callback(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
	menu_cell_basic_header_draw(ctx, cell_layer, "Maschinen");
}


static void menu_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {

	menu_cell_basic_draw(ctx, cell_layer, (const char*)menu_items[cell_index->row].title, (const char*)menu_items[cell_index->row].subtitle, NULL);

}


void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
	countdown_window_push((const char*)menu_items[cell_index->row].title);
}



#ifdef PBL_ROUND
static int16_t get_cell_height_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *ctx) {
	if (menu_layer_is_index_selected(menu_layer, cell_index)) {
		switch (cell_index->row) {
			case 0:
				return MENU_CELL_ROUND_FOCUSED_SHORT_CELL_HEIGHT;
				break;
			default:
				return MENU_CELL_ROUND_FOCUSED_TALL_CELL_HEIGHT;
		}
	}
	else {
		return MENU_CELL_ROUND_UNFOCUSED_SHORT_CELL_HEIGHT;
	}
}
#endif


static void window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	menu_layer = menu_layer_create(bounds);
	menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks) {
			.get_num_sections = menu_get_num_sections_callback,
			.get_num_rows = menu_get_num_rows_callback,
			.get_header_height = PBL_IF_RECT_ELSE(menu_get_header_height_callback, NULL),
			.draw_header = PBL_IF_RECT_ELSE(menu_draw_header_callback, NULL),
			.draw_row = menu_draw_row_callback,
			.select_click = menu_select_callback,
			.get_cell_height = PBL_IF_ROUND_ELSE(get_cell_height_callback, NULL)
		});

	menu_layer_set_click_config_onto_window(menu_layer, window);
	layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
}

static void window_unload(Window *window) {
	menu_layer_destroy(menu_layer);
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

