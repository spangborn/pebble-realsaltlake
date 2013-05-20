#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xE4, 0x5E, 0x8C, 0xCE, 0x84, 0x7D, 0x46, 0x0A, 0x80, 0x4B, 0xFE, 0x5F, 0x84, 0x85, 0x3B, 0x04 }
PBL_APP_INFO(MY_UUID,
             "Real Salt Lake", "Anesti Creative",
             1, 0, /* App version */
             RESOURCE_ID_MENU_ICON_REALSALTLAKE,
             APP_INFO_WATCH_FACE);

// App-specific data
Window window; // All apps must have at least one window
TextLayer timeLayer; // The clock
BmpContainer background_image;

void update_display(PblTm *tick_time) {

  	static char time_text[] = "00:00"; // Needs to be static because it's used by the system later.
	char *time_format;

	if (clock_is_24h_style()) {
	    time_format = "%R";
    } else {
	    time_format = "%I:%M";
    }


  string_format_time(time_text, sizeof(time_text), time_format, tick_time);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  /*if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }*/

  text_layer_set_text(&timeLayer, time_text);

}

void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {

	(void)ctx;
	update_display(t->tick_time);
  
}

void handle_init(AppContextRef ctx) {
  (void)ctx;

	window_init(&window, "Real Salt Lake");
	window_stack_push(&window, true /* Animated */);
	window_set_background_color(&window, GColorBlack);
	
	resource_init_current_app(&APP_RESOURCES);
	
	bmp_init_container(RESOURCE_ID_RSL_BACKGROUND, &background_image);
	layer_add_child(&window.layer, &background_image.layer.layer);
	
	text_layer_init(&timeLayer, GRect(30, 11, 89 /* width */, 30 /* height */));
	text_layer_set_text_color(&timeLayer, GColorWhite);
	text_layer_set_background_color(&timeLayer, GColorClear);
	text_layer_set_font(&timeLayer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROTHWELL_SUBSET_48)));
	layer_add_child(&window.layer, &timeLayer.layer);
	
	PblTm tick_time;
	get_time(&tick_time);
	update_display(&tick_time);
}
void handle_deinit(AppContextRef ctx) {
  (void)ctx;

  bmp_deinit_container(&background_image);
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
	.deinit_handler = &handle_deinit,

    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
