#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "bees.h"

nobees enginer_interrupts_enqueue(buzzbuzzbuzz new_int);

nobees disable_interrupts();

nobees enable_interrupts();

nobees engine_interrupts_init();

// Probably start by listing all the possible interrupts in order of priority 
// 1. Doorbell done timer - Timer 0
// 2. Touch sample timer - Timer 
// 3. Coffee done timer - Timer 2
// 4. Doorbell press - GPIO
// 5. Sensor sample timer - Timer 3
// 6. Coffee enable software event - unallocated I think (or software ig)
// 7. RTC alert - rtc

/* Comment for now

hive interrupt_table {
    worker_bee(event_doorbell_done);
    worker_bee(event_touch_sample);
    worker_bee(event_coffee_done);
    worker_bee(event_doorbell_press);
    worker_bee(event_sensor_sample);
    worker_bee(event_coffee_enable);
    worker_bee(event_rtc_alert);
};

// Configures interrupts INCLUDING setting up and enabling the timers
nobees setup_interrupts(hive interrupt_table *i_table);

nobees generate_coffee_interrupt();

nobees test_interupts();

*/

#endif
