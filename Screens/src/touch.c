#include "touch.h"
#include "lpc24xx.h"

#define CS_PIN            0x00100000        //P0.20

static unsigned char touch_read(unsigned char command);

void touch_init(void)
{
	// no need for PINSEL since CS_TP is GPIO by defult
	FIO0DIR |= CS_PIN;				// Set the CS_TP as an output 
	FIO0SET = CS_PIN;					// Set the CS_TP pin as high (i.e. off)
	
	S0SPCCR = 0x24; 					// set clock counter cycle 
	S0SPCR = 0x093C; 					// configures the SPI control register with required specs  
	
	PINSEL1 = 0xF<<2;					//activate MISO and MOSI
	PINSEL0 |= 0xC << 27;			// turn on SCK as pin output 
}

void touch_read_xy(char *x, char* y, char *z1, char *z2)
{
	//Read X co-ordinate from the touch screen controller
	*x = touch_read(0xD8);
	//Read Y co-ordinate from the touch screen controller
	*y = touch_read(0x98);
	//Read Z coordinates from the touch screen controller
	*z1 = touch_read(0xB8);
	*z2 = touch_read(0xC8);
}

static unsigned char touch_read(unsigned char command)
{
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
	return (unsigned char) result;
}	

