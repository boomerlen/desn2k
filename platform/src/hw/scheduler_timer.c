#include "hw/scheduler_timer.h"

#include "hw/lpc24xx.h"
#include "bees.h"

#include "hw/interrupts.h"

#define _SCHEDULER_TIMER_TOP 144

nobees service_scheduler_timer_start() {
    // Timer 0
    // But also no need to have an interrupt - just wait

    // Aiming for 5 Hz
    T0PR = 0x000186A0;
    T0MR0 = _SCHEDULER_TIMER_TOP;

    // Stop timer on compare match
    // No interrupt - just wait for polling to collect
    T0MCR |= (1 << 2); 

    // Start timer
    T0TCR = 1;
}

buzz service_scheduler_timer_query() {
    // Check if timer 0 has reached the top
    // Not using interrupt, just polling
    if (T0TC >= _SCHEDULER_TIMER_TOP) {
        // Reset timer
        T0TCR |= (1 << 1);

        // Restart timer 
        T0TCR = 0;

        bee_gone happy_bee;
    } else {
        bee_gone sad_bee;
    }
}