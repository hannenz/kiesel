#include "../kiesl.h"
#include "workout_window.h"
#include "countdown_window.h"
#include "result_window.h"

static TextLayer *text_layer;
static Layer *canvas;
int elapsed;
static char buffer[32];
extern int count;

typedef enum {
	STATE_COUNTDOWN,
	STATE_WORKOUT
} State;

typedef enum {
	PHASE_ACTIVE,
	PHASE_HOLD
} Phase;

static State state = STATE_COUNTDOWN;
static Phase phase = PHASE_ACTIVE;

void workout_update_clock() {

	text_layer_set_text_color(text_layer, (phase == PHASE_HOLD) ? GColorWhite : GColorBlack);
	text_layer_set_background_color(text_layer, (phase == PHASE_HOLD) ? GColorBlack : GColorWhite);

	snprintf(buffer, sizeof(buffer), "%u", elapsed);
	text_layer_set_text(text_layer, buffer);
}



static void layer_update_proc(Layer *layer, GContext *ctx) {
	GRect bounds = layer_get_bounds(layer);
	int angle = elapsed * 3;
	GRect frame = grect_inset(bounds, GEdgeInsets(4));

	if (phase == PHASE_ACTIVE) {
		graphics_context_set_fill_color(ctx, GColorWhite);
		graphics_fill_rect(ctx, bounds, 0, GCornerNone);
		graphics_context_set_fill_color(ctx, GColorBlack);
	}
	else {
		graphics_context_set_fill_color(ctx, GColorBlack);
		graphics_fill_rect(ctx, bounds, 0, GCornerNone);
		graphics_context_set_fill_color(ctx, GColorWhite);
	}
	graphics_fill_radial(ctx, frame, GOvalScaleModeFitCircle, 20, 0, DEG_TO_TRIGANGLE(angle));
}

static void workout_tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	if (state == STATE_COUNTDOWN) {
		elapsed--;
		if (elapsed == 0) {
			state = STATE_WORKOUT;
			phase = PHASE_ACTIVE;
			vibes_long_pulse();
		}
	}
	else {
		elapsed++;

		if (elapsed % 6 == 0) {
			vibes_long_pulse();
			phase = PHASE_ACTIVE;
		}

		if ((elapsed + 2)  % 6 == 0) {
			vibes_short_pulse();
			phase = PHASE_HOLD;
		}

		if (elapsed == 90 || elapsed == 120) {
			vibes_double_pulse();
		}

		if (elapsed == 999) {
			// abort
			countdown_select_click_handler();
		}

	}

	layer_mark_dirty(canvas);
	workout_update_clock();
}


static void countdown_select_click_handler(ClickRecognizerRef recognizer, void *context) {
	tick_timer_service_unsubscribe();
	result_window_push();
}

static void countdown_click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, countdown_select_click_handler);
}


static void window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	text_layer = text_layer_create(GRect((bounds.size.w - WIDTH) / 2, (bounds.size.h - HEIGHT) / 2, WIDTH, HEIGHT));
	text_layer_set_text(text_layer, "Start workout");
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));

	canvas = layer_create(bounds);
	layer_set_update_proc(canvas, layer_update_proc);
	layer_add_child(window_layer, canvas);
	layer_add_child(window_layer, text_layer_get_layer(text_layer));

	// Set elapsed to countdown start value
	elapsed = count;
	state = STATE_COUNTDOWN;
	phase = PHASE_ACTIVE;

	tick_timer_service_subscribe(SECOND_UNIT, workout_tick_handler);
}

static void window_unload(Window *window) {
	window_destroy(window);
	tick_timer_service_unsubscribe();
}

void workout_window_push() {
	Window *window;
	window = window_create();
	window_set_click_config_provider(window, (ClickConfigProvider)countdown_click_config_provider);
	window_set_window_handlers(window, (WindowHandlers) {
			.load = window_load,
			.unload = window_unload
	});

	
	window_stack_push(window, true);
	workout_update_clock();
}


