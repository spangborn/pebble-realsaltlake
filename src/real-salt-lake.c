#include <pebble.h>

static Window *window;
static Layer *window_layer;
static TextLayer *timeLayer; // The clock
static TextLayer *am_pmLayer; // AM/PM
static BitmapLayer *background_layer;
static GBitmap *background_image;
static GFont *time_font;
static GFont *ampm_font;

static void draw_display() {
  window_layer = window_get_root_layer(window);
  window_set_background_color(window, GColorBlack);
  
  GRect background_frame = (GRect) {
    .origin = {0,0},
    .size = {144,168}
  };
  background_layer = bitmap_layer_create(background_frame);
  background_image = gbitmap_create_with_resource(RESOURCE_ID_RSL_BACKGROUND);
  bitmap_layer_set_bitmap(background_layer, background_image);
  layer_add_child(window_layer, bitmap_layer_get_layer(background_layer));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Pushed background layer: %p", background_layer);

  timeLayer = text_layer_create(GRect(30, 0, 110 /* width */, 50 /* height */));
  text_layer_set_background_color(timeLayer, GColorClear);
  text_layer_set_text_color(timeLayer, GColorWhite);
  time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROTHWELL_SUBSET_42));
  text_layer_set_font(timeLayer, time_font);
  layer_add_child(window_layer, text_layer_get_layer(timeLayer));

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Pushed time layer: %p", timeLayer);
  
  am_pmLayer = text_layer_create(GRect(103, 10, 40 /* width */, 30 /* height */));
  text_layer_set_background_color(am_pmLayer, GColorClear);
  text_layer_set_text_color(am_pmLayer, GColorWhite);
  ampm_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROTHWELL_SUBSET_30));
  text_layer_set_font(am_pmLayer, ampm_font); 

  layer_add_child(window_layer, text_layer_get_layer(am_pmLayer));

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Pushed time layer: %p", timeLayer);

}

static void update_display(struct tm *tick_time) {

    static char time_text[] = "00:00"; // Needs to be static because it's used by the system later.
  static char ampm_text[] = " ";
  
  char *time_format;

  if (clock_is_24h_style()) {
      time_format = "%R";
    } else {
      time_format = "%I:%M";
    }
  strftime(time_text, sizeof(time_text), time_format, tick_time);
  strftime(ampm_text, sizeof(ampm_text), "%p", tick_time);

  text_layer_set_text(timeLayer, time_text);
  text_layer_set_text(am_pmLayer, ampm_text);
}

static void handle_tick (struct tm *tick_time, TimeUnits units_changed) {
  
  // If the minute was what changed
  update_display(tick_time);
  
}

static void window_load (Window *window) {

  // Actually draw
  draw_display();
}

static void window_unload (Window *window) {
  // Nuke the layers
  bitmap_layer_destroy(background_layer);

  text_layer_destroy(timeLayer);
  text_layer_destroy(am_pmLayer);
}
void init(void) {

  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);

  draw_display();

}
void deinit(void) {

  text_layer_destroy(am_pmLayer);
  text_layer_destroy(timeLayer);
  bitmap_layer_destroy(background_layer);
  gbitmap_destroy(background_image);

}

int main (void) {
  init();
  tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
