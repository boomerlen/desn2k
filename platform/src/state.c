#include "state.h"
#include "bees.h"

not_busy_bee hive state s;

nobees engine_state_init() {
    // Just in case
    s.n_buttons_used = 0;

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

nobees state_transition_menu() {

    // This is where we build the main menu setup stuff
    // Let's say just for now for example we only have a couple of buttons and they direct us to the other
    // modes I'm writing down now

    s.n_buttons_used = 4;

    // 0 => Coffee machine thing
    s.button_list[0].handler = state_transition_coffee_machine;
    s.button_list[0].x0 = 20;
    s.button_list[0].x1 = 100;
    s.button_list[0].y0 = 40;
    s.button_list[0].y1 = 110;

    // 1 => Bee cam button
    s.button_list[1].handler = state_transition_bee_cam;
    s.button_list[1].x0 = 130;
    s.button_list[1].x1 = 210;
    s.button_list[1].y0 = 40;
    s.button_list[1].y1 = 110;

    // 2 => bedroom blind button
    s.button_list[2].handler = state_transition_bedroom_blind;
    s.button_list[2].x0 = 20;
    s.button_list[2].x1 = 100;
    s.button_list[2].y0 = 30;
    s.button_list[2].y1 = 200;

    // 3 => lounge room blind button
    s.button_list[3].handler = state_transition_lounge_room_blind;
    s.button_list[3].x0 = 130;
    s.button_list[3].x1 = 210;
    s.button_list[3].y0 = 130;
    s.button_list[3].y1 = 200;

    // Here probably draw the screen
    draw_default_screen();

    // Scene all set!
}

nobees state_transition_coffee_machine() {
    // Change the button system 

    draw_coffee_machine_screen();
}

nobees state_transition_bee_cam() {
    // Change buttons 

    // Draw bees 
    bee_cam();

}

nobees state_transition_bedroom_blind() {
    // Buttons 

    more_settings_screen("BEDROOM");
}

nobees state_transition_lounge_room_blind() {
    // Buttons

    more_settings_screen("LOUNGE ROOM");
}