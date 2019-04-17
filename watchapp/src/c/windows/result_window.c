#include "../kiesl.h"
#include "result_window.h"
#include "menu_window.h"

static TextLayer *text_layer;

char buffer[32];

extern int elapsed;



void result_update_clock() {

	snprintf(buffer, sizeof(buffer), "%u", elapsed);
	text_layer_set_text(text_layer, buffer);

}



static void result_select_click_handler(ClickRecognizerRef recognizer, void *context) {
	window_stack_pop(false); // result window
	window_stack_pop(false); // workout window
	window_stack_pop(false); // countdown window
	window_stack_pop(false); // menu window
	menu_window_push();
}



static void result_up_click_handler(ClickRecognizerRef recognizer, void *context) {
	elapsed++;
	result_update_clock();
}



static void result_down_click_handler(ClickRecognizerRef recognizer, void *context) {
	if (elapsed > 1) {
		elapsed--;
	}
	result_update_clock();
}



static void result_click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, result_select_click_handler);
	window_single_click_subscribe(BUTTON_ID_UP, result_up_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, result_down_click_handler);
}



static void window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	text_layer = text_layer_create(GRect((bounds.size.w - ACTION_BAR_WIDTH - WIDTH) / 2, (bounds.size.h - HEIGHT) / 2, WIDTH, HEIGHT));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	layer_add_child(window_layer, text_layer_get_layer(text_layer));

	ActionBarLayer *action_bar; 
	action_bar = action_bar_layer_create();
	action_bar_layer_add_to_window(action_bar, window);
	action_bar_layer_set_click_config_provider(action_bar, (ClickConfigProvider)result_click_config_provider);

	GBitmap *increment_icon, *decrement_icon, *accept_icon;
	increment_icon = gbitmap_create_with_resource(RESOURCE_ID_ICON_INCREMENT);
	decrement_icon = gbitmap_create_with_resource(RESOURCE_ID_ICON_DECREMENT);
	accept_icon = gbitmap_create_with_resource(RESOURCE_ID_ICON_ACCEPT);
	action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_UP, increment_icon, true);
	action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_DOWN, decrement_icon, true);
	action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_SELECT, accept_icon, true);


	result_update_clock();
}



static void window_unload(Window *window) {
	text_layer_destroy(text_layer);
	window_destroy(window);
}



void result_window_push() {
	Window *window;

	window = window_create();
	window_set_click_config_provider(window, (ClickConfigProvider)result_click_config_provider);
	window_set_window_handlers(window, (WindowHandlers) {
			.load = window_load,
			.unload = window_unload
	});

	window_stack_push(window, true);
}


