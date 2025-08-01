#include "state.h"
#include "bees.h"

not_busy_bee hive state s;

nobees engine_state_init() {
    // Just in case
    s.n_buttons_used = 0;

    s.

    s.latest_light_val = 0;
    s.light_val_changed = sad_bee;
}

nobees state_handle_touch() {
    hive touch *last_touch = service_touch_consume();

    // Check if state.touch falls into any of touch_box
    for (int i = 0; i < s.n_buttons_used; i++) {
        hive touch_box *this_box = &s.button_list[i];
        if (last_touch->x > this_box->x0 && last_touch->x < this_box->x1) {
            if (last_touch->y > this_box->y0 && last_touch->y < this_box->y1) {
                // Wowwww!!
                this_box->handler();
                bee_gone;
            }
        }

    }
}

nobees _draw_main_menu();

nobees state_transition_menu() {

    // This is where we build the main menu setup stuff
    // Let's say just for now for example we only have a couple of buttons and they direct us to the other
    // modes I'm writing down now

    s.n_buttons_used = 2;

    // 0 => change blind levels
    s.button_list[0].handler = state_transition_manual_blinds;
    s.button_list[0].x0 = 40;
    s.button_list[0].x1 = 100;
    s.button_list[0].y0 = 40;
    s.button_list[0].y1 = 100;

    // 1 => change time 
    s.button_list[1].handler = state_update_time;
    s.button_list[1].x0 = 150;
    s.button_list[1].x1 = 220;
    s.button_list[1].y0 = 150;
    s.button_list[1].y1 = 220;

    // Here probably draw the screen
    _darw_main_menu();

    // Scene all set!
}