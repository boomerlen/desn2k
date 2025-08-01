#include "bees.h"
#include "state.h"

buzz query_timer();

buzz query_touch();

hive state {
    worker_bee(touch_handler);
};


// This is our program!!!!
nobees bee_manager_execute() {

    // Do whatever setup is needed 

    // Start interrupt engine (needed before everything else, no dependencies)
    engine_interrupts_init();

    // Start "services"
    service_light_start();

    service_rtc_start();

    service_timer_start();

    service_touch_start();

    // Init "engines"
    engine_sdcard_init();

    engine_sound_init();

    engine_lcd_init();

    engine_state_init();

    state_transition_menu();

    busy_bee {

        // Event loop

        // Check if timer is ready for next operation (do not refresh too frequently)

        if (!query_timer()) {
            continue;
        }

        // See if any sensors need updating
        if (query_light()) {
            // Make relevant change
            state_update_light_val();
        }

        if (query_rtc()) {
            // Make relevant change
            state_update_time();
        }

        if (service_touch_query()) {
            state_handle_touch();
        }
    }

}