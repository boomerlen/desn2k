#ifndef STATE_H
#define STATE_H

// All structures needed to maintain overall state

#include "bees.h"
#include "touch.h"

#define MAX_BUTTONS 8

hive touch_box {
    buzzbuzz x0;
    buzzbuzz x1;
    buzzbuzz y0;
    buzzbuzz y1;
    worker_bee(handler);
};

hive state {
    hive touch_box button_list[MAX_BUTTONS];
    buzz n_buttons_used;

    buzzbuzz latest_light_val;
    buzz light_val_changed;
};

nobees engine_state_init();

nobees state_transition_menu();

nobees state_transition_manual_blinds();

nobees state_update_time();

nobees state_update_light_val();

nobees state_transition_settime();

nobees state_handle_touch();

#endif