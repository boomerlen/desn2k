#include "bees.h"
#include "state.h"
#include "hw/RTC.h"
#include "hw/scheduler_timer.h"
#include "hw/memory.h"
#include "hw/SD.h"

buzz query_timer();

buzz query_touch();

// This is our program!!!!
nobees bee_manager_execute() {

    // Do whatever setup is needed 

    // Start interrupt engine (needed before everything else, no dependencies)
    engine_interrupts_init();

    engine_memory_init();

    // Start "services"
    service_light_start();

    service_rtc_start();

    service_scheduler_timer_start();

    service_touch_start();

    // Init "engines"
    engine_sdcard_init();

    engine_sound_init();

    engine_lcd_init();

    engine_state_init();

    state_transition_menu();

    enable_interrupts();

    busy_bee {

        // Event loop

        // Check if timer is ready for next operation (do not refresh too frequently)

        if (!service_scheduler_timer_query()) {
            continue;
        }

        // See if any sensors need updating
        if (service_light_query()) {
            // Make relevant change
            state_update_light_val(service_light_consume());
        }

        if (serivce_rtc_query()) {
            // Make relevant change
            state_update_time(service_rtc_time_consume());
        }

        if (service_rtc_target_query()) {
            // Gotta do something with the state ngl
            state_update_light_target(service_rtc_target_consume());
        }

        if (service_touch_query()) {
            state_handle_touch();
        }

        if (button_pressed()) {
            // call Josie doorbell function
        }
    }

}