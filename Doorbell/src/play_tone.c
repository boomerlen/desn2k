#include "lpc24xx.h"

// Initialize DAC.
// This should be the same as the previous exercise.
void setup_DAC(void) {
	// Setup Pin Connect for AOUT
	FIO0DIR |= 0x4000000; // set up pin 0.26 to output 
	PINSEL1 |= 0x200000;
}

// udelay should delay the processor for 'delay_in_us' number of microseconds.
// * LPC24XX.h contains a definition for the PCLK, "Fpclk"
//   use this definition for PCLK as defined in the manual.
// * Use Timer 0. This means you MUST use the prefix T0 for every control
//   register, i.e. T0TCR.
// * Make sure you reset the timer, and when you start the timer ENSURE you
//   set the reset bit back to zero!
void udelay(unsigned int delay_in_us) {
	T0TCR |= 0x3;												// Resets timer												
	T0TCR &= 0xFD;											// sets reset back to zero 
	T0MR0 = delay_in_us*72;	// Sets check value
	T0MCR = 0x4;												// Sets match control to reset
	
	while(T0TCR & 0x01){
	}
	
	T0TCR |= 0x2;
	T0TCR &= 0xFC;
}

// play_tone should play a tone for 'duration' number of microseconds at the pitch
// given by 1/'period'. Played at the volume given by 'vol', which ranges from																   
// 0 (off) to 0x3FF (max volume).																		   

void play_tone(unsigned int duration, int period, int vol) {
	int i = 0;
	
	while (i < duration){
		DACR = vol<<6;
		udelay(period/2);
		DACR = 0<<6;
		udelay(period/2);
		i = i + period;
	}
}
