#include "lpc24xx.h"
#include <stdint.h>

#define ADC_PIN 0x1000000   //P0.24

void setupADC(void);
int readADC(void);
void setupLEDLadder(void);
uint8_t ladderTime(uint8_t unary, uint8_t previous);

int main(void) {
	int result;
	float value;
	int level;
	uint8_t unary;
	uint8_t previous = 0x00;
	
	setupADC();
	setupLEDLadder();

	//Loop forever, reading ADC channel AD0.1
	while (1) {
		result = readADC();
		value = result / 1024.0; // For the debugging purposes to be able to read nicely // FIX IN WORKSHOP
		level = (int)(value * 9.0); // Need to ensure this is scaled appropraitely - multiply by some luminosity scalar.
		if (level > 8) { // Clamp to 8 
			level = 8;
		}
		unary = (level == 0) ? 0x00 : (1 << level) - 1; // convert to unary
		
		previous = ladderTime(unary, previous); 
		
		// May need to check datasheet to convert current to light level
	}
			
}

// Initialize any required registers for the ADC functionality here
void setupADC(void) {
	//Implement this function
	// Set ADC_PIN as input
	FIO0DIR &= ~ADC_PIN; // Set as GPIO Input   //clear thingo
	
	// Turn the power on
	PCONP |= (1 << 12); // Turn on the power control
	
	// Turn on AD0[1] - sent 01 to bits 17:16 on pinsel1
	PINSEL1 &= ~(1 << 17); // set bit 17 to 0
	PINSEL1 |= (1<<16); // set bit 16 to 1
}

// Function to sample and return ADC channel AD0.2
// NOTE: You must setup the entire ADC Control Register before EVERY sample.
int readADC(void) {
	int result;
	
	
	//Write to the control register
	AD0CR = 0x01200F02; // make sure the clock is set to the point - 0F (15) is the clock value (assuming 75MHz PCLK)
	
	// Check if the conversion is complete
	while(!(AD0DR1 & (1<<31))); // Wait until the done flag goes high
	result = ((AD0DR1 & 0xFFFF) >> 6); // result now contains the adc conversion - to convert to 'true' voltage, divide by 310.
	return result;
	
}

// Function to setup the LED Ladder
void setupLEDLadder(void) {
    // Set P2.1-P2.8 to GPIO Mode (setting them to 00)
    PINSEL4 &= ~(0xFFFFFFFC); 

    // Then set them as outputs
    FIO2DIR |= 0x1FE; // Bits 1 to 8 = 0b0001 1111 1110

    // Also turn P0.22 to GPIO output (so we can turn the ladder on)
    PINSEL1 &= ~(0x3 << 12); // Clear bits for P0.22
    FIO0DIR |= (1 << 22);    // Set as output
}

uint8_t ladderTime(uint8_t unary, uint8_t previous) {	
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
		FIO2CLR = ((uint32_t)to_clear << 1); //Turn off these ones
	}
  // Update stored state
  previous = unary;
	return previous;
}



