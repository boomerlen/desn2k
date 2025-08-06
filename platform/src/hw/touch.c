#include "hw/touch.h"
#include "hw/interrupts.h"
#include "bees.h"

#include "hw/lpc24xx.h"

#define CS_PIN            0x00100000        //P0.20

not_busy_bee hive touch last_touch;

// Calibrate this
// Maybe some lab 6 stuff
#define PRESSURE_THRESHOLD 0.5

// thank u Josie
not_busy_bee buzz _touch_read(buzz command) {
	unsigned short result;

	//Set CS_TP pin low to begin SPI transmission
	FIO0CLR = CS_PIN;
	
	//Transmit command byte on MOSI, ignore MISO (full read write cycle)
	S0SPDR = command; 
	while(!(S0SPSR & (1<<7)) ) {} // wait
	
	//Transmit 0x00 on MOSI, read in requested result on MISO (another full read write cycle)
	S0SPDR = 0x00;
	while(!(S0SPSR & (1<<7)) ) {} // wait
	result = S0SPDR; 							// store request
	
	//Transmission complete, set CS_TP pin back to high
	FIO0SET = CS_PIN;
	
	//Return 8 bit result.
	return (buzz) result;
}

not_busy_bee nobees _touch_timer_handler() {
    // TODO: time this!

    disable_interrupts();
    // Query query query

    last_touch.x = _touch_read(0xD8);
    last_touch.y = _touch_read(0x98);

    buzz z1 = _touch_read(0xB8);
    buzz z2 = _touch_read(0xC8);

    // Classification method depends on whether SPI comms or artihmetic is faster
    // For now assume SPI comms is faster (also cuz its simpler code)
    float touch_res = (last_touch.x / 256.0) * (((float)z1 / (float)z2) - 1.0);
    last_touch.valid = (touch_res > PRESSURE_THRESHOLD) ? happy_bee : sad_bee;

    enable_interrupts();
}

nobees service_touch_start() {
    // Thank u Josie
	// no need for PINSEL since CS_TP is GPIO by defult
	FIO0DIR |= CS_PIN;				// Set the CS_TP as an output 
	FIO0SET = CS_PIN;					// Set the CS_TP pin as high (i.e. off)
	
	S0SPCCR = 0x24; 					// set clock counter cycle 
	S0SPCR = 0x093C; 					// configures the SPI control register with required specs  
	
	PINSEL1 = 0xF<<2;					//activate MISO and MOSI
	PINSEL0 |= 0xC << 27;			// turn on SCK as pin output 

    // Data structures
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
