#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_time_layer_seconds;
static TextLayer *s_wday_layer;
static TextLayer *s_date_layer;

static char * wDays[] = {
  "Sonntag",
  "Montag",
  "Dienstag",
  "Mittwoch",
  "Donnerstag",
  "Freitag",
  "Samstag"
};

// static TextLayer *s_ac_time_layer;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL) - 180;
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00 ";
  static char bufferS[] = "00";
  static char bufferWD[] = "0";
  static char bufferDT[] = "00.00";
  
//  static char buffer_ac[] = "AC: 00:00 ";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00 "), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00 "), "%I:%M", tick_time);
  }
  strftime(bufferS, sizeof("00"), "%S", tick_time);
  strftime(bufferWD, sizeof("0"), "%w", tick_time);
  strftime(bufferDT, sizeof("00.00"), "%d.%m", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(s_time_layer_seconds, bufferS);
  text_layer_set_text(s_wday_layer, wDays[atoi(bufferWD)]);
  text_layer_set_text(s_date_layer, bufferDT);
  
//  time_t temp_ac = time(NULL) - 21600;
//  struct tm *tick_time_ac = localtime(&temp_ac);
  
//  if(clock_is_24h_style() == true) {
    // Use 24 hour format
//    strftime(buffer_ac, sizeof("AC: 00:00 "), "WF: %H:%M", tick_time_ac);
//  } else {
    // Use 12 hour format
//    strftime(buffer_ac, sizeof("AC: 00:00 "), "WF: %I:%M", tick_time_ac);
//  }
  
//  text_layer_set_text(s_ac_time_layer, buffer_ac);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  // Create Background
  s_time_layer = text_layer_create(GRect(0, 0, 144, 168));
  s_time_layer_seconds = text_layer_create(GRect(0, 145, 144, 168));
  s_wday_layer = text_layer_create(GRect(2, 0, 100, 20));
  s_date_layer = text_layer_create(GRect(100, 0, 40, 20));
  text_layer_set_background_color(s_time_layer, GColorBlack);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorClear);
  text_layer_set_background_color(s_time_layer_seconds, GColorBlack);
  text_layer_set_text_color(s_time_layer_seconds, GColorClear);
  text_layer_set_background_color(s_wday_layer, GColorBlack);
  text_layer_set_text_color(s_wday_layer, GColorClear);
  text_layer_set_background_color(s_date_layer, GColorBlack);
  text_layer_set_text_color(s_date_layer, GColorClear);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_text(s_time_layer_seconds, "00");
  text_layer_set_text(s_wday_layer, "Tag");
  text_layer_set_text(s_date_layer, "00.00");
  
//  s_ac_time_layer = text_layer_create(GRect(0, 120, 144, 22));
//  text_layer_set_background_color(s_ac_time_layer, GColorBlack);
//  text_layer_set_text_color(s_ac_time_layer, GColorClear);
//  text_layer_set_text_alignment(s_ac_time_layer, GTextAlignmentCenter);
//  text_layer_set_text(s_ac_time_layer, "AC: 00:00");

  // Improve the layout to be more like a watchface
  GFont s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SEGOE_UI_NORMAL_48));
  GFont s_time_font_small = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SEGOE_UI_NORMAL_14));
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_font(s_time_layer_seconds, s_time_font_small);
  text_layer_set_font(s_wday_layer, s_time_font_small);
  text_layer_set_font(s_date_layer, s_time_font_small);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_time_layer_seconds, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentRight);
  
//  text_layer_set_font(s_ac_time_layer, s_time_font_small);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer_seconds));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_wday_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  
//  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_ac_time_layer));
  
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  update_time();
}

static void main_window_unload(Window *window) {
    // Destroy TextLayer
    text_layer_destroy(s_time_layer);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}