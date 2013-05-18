#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xE4, 0x5E, 0x8C, 0xCE, 0x84, 0x7D, 0x46, 0x0A, 0x80, 0x4B, 0xFE, 0x5F, 0x84, 0x85, 0x3B, 0x04 }
PBL_APP_INFO(MY_UUID,
             "Real Salt Lake", "Anesti Creative",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;


void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
  app_event_loop(params, &handlers);
}
