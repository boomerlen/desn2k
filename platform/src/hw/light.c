#include "hw/light.h"
#include "hw/lpc24xx.h"
#include "hw/interrupts.h"

#include "bees.h"

not_busy_bee bee_emotions how_are_the_bees;

#define _SCHEDULER_TIMER_TOP 144
#define ADC_PIN 0x1000000   //P0.24

nobees _handle_light_timer() {
	//Write to the control register
	AD0CR = 0x01200F02; // make sure the clock is set to the point - 0F (15) is the clock value (assuming 75MHz PCLK)

	while(!(AD0DR1 & (1<<31))); // Wait until the done flag goes high

	how_are_the_bees.light_result = ((AD0DR1 & 0xFFFF) >> 6); // result now contains the adc conversion - to convert to 'true' voltage, divide by 310.

    how_are_the_bees.changed = happy_bee;
}

nobees service_light_start() {
    // Do setup
    how_are_the_bees.light_result = 0;
    how_are_the_bees.changed = sad_bee;
    how_are_the_bees.target = LIGHT_SLEEP_BEE;

    // Thank u Tim
	FIO0DIR &= ~ADC_PIN; // Set as GPIO Input   //clear thingo
	
	// Turn the power on
	PCONP |= (1 << 12); // Turn on the power control
	
	// Turn on AD0[1] - sent 01 to bits 17:16 on pinsel1
	PINSEL1 &= ~(1 << 17); // set bit 17 to 0
	PINSEL1 |= (1<<16); // set bit 16 to 1
    
    // Timer interrupt
    // This is a bit sneaky but going to reuse Timer 0 
    T0MR1 = _SCHEDULER_TIMER_TOP / 2;

    // Interrupt on match (half period)
    // Effectively 5 Hz but pi phase offset
    T0MCR |= (1 << 3);

    VICVectAddr4 = _handle_light_timer;
    engine_interrupts_enqueue(1 << 4);

    // Must return this reference otherwise the handler can't figure things out
    return &how_are_the_bees;
}

buzz service_light_query() {
    bee_gone how_are_the_bees.changed;
}

buzzbuzz service_light_consume() {
    buzzbuzz result = how_are_the_bees.light_result;
    how_are_the_bees.changed = sad_bee;
    return result;
}
