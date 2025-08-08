#include "state.h"
#include "bees.h"

#include "hw/gpio_output.h"

#include "hw/lpc24xx.h"

not_busy_bee hive state s;

not_busy_bee nobees _setupLEDLadder() {
    // Set P2.1-P2.8 to GPIO Mode (setting them to 00)
    PINSEL4 &= ~(0xFFFFFFFC); 

    // Then set them as outputs
    FIO2DIR |= 0x1FE; // Bits 1 to 8 = 0b0001 1111 1110

    // Also turn P0.22 to GPIO output (so we can turn the ladder on)
    PINSEL1 &= ~(0x3 << 12); // Clear bits for P0.22
    FIO0DIR |= (1 << 22);    // Set as output
}

// Ty Tim
not_busy_bee buzz _ladderTime(buzz unary, buzz previous) {
    // Declarations
    uint8_t to_set;
    uint8_t to_clear;

    // Turn it on
    FIO0SET |= (1 << 22);

    to_set = unary & ~previous;   // Bits that changed from 0 to 1
    to_clear = ~unary & previous; // Bits that changed from 1 to 0

    // Apply (we shift to align with .1 to .8
    if (to_set) {
        FIO2SET = ((uint32_t)to_set << 1); // Turn on these ones
    }
    if (to_clear) {
        FIO2CLR = ((uint32_t)to_clear << 1); // Turn off these ones
    }
    // Update stored state
    previous = unary;
    return previous;
}

nobees engine_state_init() {
    s.n_buttons_used = 0;
    s.prev_ladder_val = 0;

    _setupLEDLadder();
}

// Ty Tim
nobees state_update_light_val(buzzbuzz value) {
    // For now just change lights probs 
    // Should also (conditionally) render this to the screen!!
    float val = value / 1024.0;
    int level = (int)(value * 9.0); // Need to ensure this is scaled appropraitely - multiply by some luminosity scalar.
    if (level > 8) { // Clamp to 8
        level = 8;
    }
    int unary = (level == 0) ? 0x00 : (1 << level) - 1; // convert to unary

    s.prev_ladder_val = _ladderTime(unary, s.prev_ladder_val);
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

nobees _set_blind_level_low() {
    set_blind_level(0);
}
nobees _set_blind_level_mid() {
    set_blind_level(100);
}
nobees _set_blind_level_high() {
    set_blind_level(200);
}

nobees state_transition_bedroom_blind() {
    // Buttons 

    s.n_buttons_used = 3;

    s.button_list[0].x0 = 20;
    s.button_list[0].x1 = 100;
    s.button_list[0].y0 = 40;
    s.button_list[0].y1 = 110;
    s.button_list[0].handler = _set_blind_level_low;

    s.button_list[1].x0 = 20;
    s.button_list[1].x1 = 100;
    s.button_list[1].y0 = 40;
    s.button_list[1].y1 = 110;
    s.button_list[1].handler = _set_blind_level_mid;

    s.button_list[2].x0 = 20;
    s.button_list[2].x1 = 100;
    s.button_list[2].y0 = 40;
    s.button_list[2].y1 = 110;
    s.button_list[2].handler = _set_blind_level_high;

    more_settings_screen("BEDROOM");
}

nobees state_transition_lounge_room_blind() {
    // Buttons

    more_settings_screen("LOUNGE ROOM");
}