#include "main_window.h"
#include "countdown_window.h"
#include "menu_window.h"

static Window *window;



static void main_window_select_click_handler(ClickRecognizerRef recognizer, void *context) {
	menu_window_push();
}



static void click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, main_window_select_click_handler);
}



static void window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	TextLayer *text_layer = text_layer_create(bounds);
	text_layer_set_text(text_layer, "Welcome");
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	layer_add_child(window_layer, text_layer_get_layer(text_layer));
}



static void window_unload(Window *window) {
	window_destroy(window);
}



void main_window_push() {
	if (!window) {
		window = window_create();

		window_set_click_config_provider(window, click_config_provider);

		window_set_window_handlers(window, (WindowHandlers) {
				.load = window_load,
				.unload = window_unload
		});
	}
	
	window_stack_push(window, true);
}


