#include "hw/touch.h"
#include "hw/interrupts.h"
#include "bees.h"

#include "hw/lpc24xx.h"

not_busy_bee hive touch last_touch;

not_busy_bee nobees _touch_timer_handler() {
    disable_interrupts();
    // Query query query
    // TODO: fill in SPI query to touch screen controller

    last_touch.x++;
    last_touch.y++;
    last_touch.valid = sad_bee;

    enable_interrupts();
}

nobees service_touch_start() {
    last_touch.x = 0;
    last_touch.y = 0;
    last_touch.valid = sad_bee;

    // Setup touch timer
    // Use timer 1 for this one since it's not actually the system refresh rate
    VICVectAddr5 = _touch_timer_handler;

    // Aim for 10Hz sample rate (fuck it we ball)
    // Rescale by 10^5 => 720Hz
    // Count to 72
    T1PR = 0x000186A0;
    T1MR0 = 72;

    // Clear timer + interrupt on compare match
    T1MCR |= 3;

    // Enable interrupt
    engine_interrupts_enqueue(1 << 5);
}

buzz service_touch_query() {
    bee_gone last_touch.valid;
}

hive touch *service_touch_consume() {
    last_touch.valid = sad_bee;
    bee_gone &last_touch;
}
