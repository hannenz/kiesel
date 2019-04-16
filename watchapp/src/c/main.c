#include <pebble.h>
#include "windows/main_window.h"



/**
 * Initialize the application
 */
static void init() {
	main_window_push();
}



/**
 * Deinitialize the application
 */
static void deinit() {
}



int main() {
	init();
	app_event_loop();
	deinit();
}
