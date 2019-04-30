#include "../kiesl.h"
#include "countdown_window.h"
#include "workout_window.h"

static TextLayer *text_layer;

#define KEY_COUNTDOWN 8


int count = 3;

char buffer[8];
static char message[32];



void countdown_update_clock() {

	snprintf(buffer, sizeof(buffer), "%u", count);
	text_layer_set_text(text_layer, buffer);
}



static void countdown_select_click_handler(ClickRecognizerRef recognizer, void *context) {
	persist_write_int(KEY_COUNTDOWN, count);
	workout_window_push();
}

static void countdown_up_click_handler(ClickRecognizerRef recognizer, void *context) {
	count++;
	countdown_update_clock();
}

static void countdown_down_click_handler(ClickRecognizerRef recognizer, void *context) {
	if (count > 1) {
		count--;
	}
	countdown_update_clock();
}


static void countdown_click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, countdown_select_click_handler);
	window_single_click_subscribe(BUTTON_ID_UP, countdown_up_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, countdown_down_click_handler);
}


static void window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	TextLayer *tl1;
	tl1= text_layer_create(GRect(4, 4, bounds.size.w, 28));
	text_layer_set_text_alignment(tl1, GTextAlignmentLeft);
	text_layer_set_text(tl1, message);
	text_layer_set_font(tl1, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	layer_add_child(window_layer, text_layer_get_layer(tl1));

	text_layer = text_layer_create(GRect((bounds.size.w - ACTION_BAR_WIDTH - WIDTH) / 2, (bounds.size.h - HEIGHT) / 2, WIDTH, HEIGHT));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	text_layer_set_text(text_layer, message);
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	layer_add_child(window_layer, text_layer_get_layer(text_layer));

	ActionBarLayer *action_bar;
	action_bar = action_bar_layer_create();
	action_bar_layer_add_to_window(action_bar, window);
	action_bar_layer_set_click_config_provider(action_bar, (ClickConfigProvider)countdown_click_config_provider);

	GBitmap *increment_icon, *decrement_icon, *play_icon;

	increment_icon = gbitmap_create_with_resource(RESOURCE_ID_ICON_INCREMENT);
	decrement_icon = gbitmap_create_with_resource(RESOURCE_ID_ICON_DECREMENT);
	play_icon = gbitmap_create_with_resource(RESOURCE_ID_ICON_PLAY);
	action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_UP, increment_icon, true);
	action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_DOWN, decrement_icon, true);
	action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_SELECT, play_icon, true);

	countdown_update_clock();

}

static void window_unload(Window *window) {
	text_layer_destroy(text_layer);
	window_destroy(window);
}

void countdown_window_push(const char *mssg) {

	strncpy(message, mssg, sizeof(message));

	Window *window;
	window = window_create();
	/* window_set_click_config_provider(window, (ClickConfigProvider)countdown_click_config_provider); */
	window_set_window_handlers(window, (WindowHandlers) {
			.load = window_load,
			.unload = window_unload
	});

	if (persist_exists(KEY_COUNTDOWN)) {
		count = persist_read_int(KEY_COUNTDOWN);
	}
	else {
		count = 3;
	}
	
	window_stack_push(window, true);
}


