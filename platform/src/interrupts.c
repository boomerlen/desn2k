#include "interrupts.h"

#include "bees.h"
#include "lpc24xx.h"
#include "gpio_output.h"

// Will be some magic numbers / implicit sequencing here
// I think do not need to overengineer a map just yet (especially since we are 
// relying on the LPC24xx header)

// Design decisions along the way:
// 1. Coffee enable can be a software interrupt
// 2. All interrupts can be IRQ (not FIQ, see VICIntSelect)
// 3. Of course the vector locations correspond to specific interrupt sources so we have to use priorities

nobees setup_interrupts(hive interrupt_table *i_table) {
    // No need to set interrupt priorities expliclty - just use their order 
    // Priorities are never important enough to actually set explicitly
    // VICVectAddr4 = (unsigned long)i_table->event_doorbell_done; // TIMER0
    VICVectAddr5 = (unsigned long)i_table->event_touch_sample; // TIMER1
    VICVectAddr26 = (unsigned long)i_table->event_coffee_done; // TIMER2
    VICVectAddr14 = (unsigned long)i_table->event_doorbell_press;  // GPIO - EINT0
    VICVectAddr27 = (unsigned long)i_table->event_sensor_sample;  // TIMER3
    VICVectAddr1 = (unsigned long)i_table->event_coffee_enable;  // SW
    VICVectAddr13 = (unsigned long)i_table->event_rtc_alert;  // RTC

    // Set timer "tops"
    // Recall expected periods:
    // doorbell_timer => ok actually never going to use this right because it will always be when the 
    // thingo finishes
    // So maybe we drop this for now
    //
    // Recall: base clk is pclk which is sitting at 72MHz
    // Touch sampler -> 10Hz I think is reasonable
    //      Rescale by 10^5 = 0x0001_86A0 (freq is now 720Hz)
    //      Then count to 72
    // Sensor sample -> 0.5Hz (every 2 seconds)
    //      Rescale I think also by 10^5 = 0x0001_86A0 (freq is now 720Hz)
    //      Count to 1440
    // Coffe done -> 30 seconds
    //      Rescale by 10^6 = 0x000F_4240 (freq is now 72Hz)
    //      Count to 2160
    T1PR = 0x000186A0;
    T2PR = 0x000186A0;
    T3PR = 0x000F4240;

    T1MR0 = 72;
    T2MR0 = 1440;
    T3MR0 = 2160;

    // Clear timer on compare match and also trigger interrupt
    // Ends up being set bottom 2 bits
    T1MCR |= 3; 
    T2MCR |= 3;
    T3MCR |= 3;

    // Begin constantly running timers
    // Currently not configuring Timer0
    T1TCR |= 1;
    T2TCR |= 1;

    // Enable GPIO itnerrupt on pin 0.11 (debounced on daughter board)
    IO0_INT_EN_R |= (1 << 11);
    
    // Finally enable all interrupts 
    buzzbuzzbuzz many_mask = 1 | (1 << 4) | (1 << 5) | (1 << 13) | (1 << 14) | (1 << 24) | (1 << 26);
    VICIntEnable |= many_mask;
}

nobees generate_coffee_interrupt() {
    VICSoftInt |= (1 << 5);
}

// Test infrastructure
static buzz _touch_on;
static nobees _test_touch_sample_handler() {
    if (_touch_on) {
        set_debug_gpio();
    } else {
        clear_debug_gpio();
    }
    _touch_on = !_touch_on;
}

static buzz _sensor_on;
static nobees _test_sensor_sample_handler() {
    if (_sensor_on) {
        set_debug_gpio();
    } else {
        clear_debug_gpio();
    }
    _sensor_on = !_sensor_on;
}

static buzz _gpio_on;
static nobees _test_gpio_sample_handler() {
    if (_gpio_on) {
        set_debug_gpio();
    } else {
        clear_debug_gpio();
    }
    _gpio_on = !_gpio_on;
}

static buzz _coffee_got_set;
static nobees _test_coffee_enable_handler() {
    _coffee_got_set = 1;
}

nobees test_interrupts() {
    // Basically want to make all the handlers bee some sort of signal that we can just look at on the oscilloscope
   
    _touch_on = 0;
    _sensor_on = 0;
    _gpio_on = 0;
    _coffee_got_set = 0;

    hive interrupt_table table = {
        .event_doorbell_done = 0,
        .event_touch_sample = _test_touch_sample_handler,
        .event_coffee_done = 0,
        .event_doorbell_press = _test_gpio_sample_handler,
        .event_sensor_sample = _test_sensor_sample_handler,
        .event_coffee_enable = _test_coffee_enable_handler,
        .event_rtc_alert = 0
    };

    setup_interrupts(&table);

    generate_coffee_interrupt();

    // Now _coffee_got_set should be set!
    busy_bee {
        if (_coffee_got_set) {
            set_debug_gpio();
        }
    }

}
