#include <pebble.h>

Window *s_main_window;
TextLayer *text_layer;
static TextLayer *s_output_layer;
char* s_buffer;

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_output_layer = text_layer_create(GRect(0, 40, bounds.size.w, 50));
  text_layer_set_overflow_mode(s_output_layer, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(s_output_layer, GTextAlignmentCenter);

  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_output_layer);
}

static void load_resource() {
  // Get resource and size
  ResHandle handle = resource_get_handle(PATH_STOPS_FILE);
  size_t res_size = resource_size(handle);

  // Copy to buffer
  s_buffer = (char*)malloc(res_size);
  resource_load(handle, (uint8_t*)s_buffer, res_size);

  // Show in TextLayer
  text_layer_set_text(s_output_layer, s_buffer);
}


void handle_init(void) {
	// Create a window and text layer
	s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
	
	// Push the window
	window_stack_push(s_main_window, true);
	load_resource();
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}


void handle_deinit(void) {
	// Destroy the text layer
	//text_layer_destroy(text_layer);
	
	// Destroy the window
	window_destroy(s_main_window);
  free(s_buffer);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
