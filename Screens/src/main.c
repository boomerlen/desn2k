#include "bubbles.h"
#include "touch.h"
#include "bubbles.h"
#include "lpc24xx.h"   
#include "lcd/lcd_hw.h"
#include "lcd/lcd_grph.h"
#include "lcd/lcd_cfg.h"
#include "lcd/sdram.h"
#include "delay.h"
#include <stdlib.h>

void default_screen(void);
void non_default_screen(void);

int main(void) {

	//Setup external SDRAM. Used for Frame Buffer
	sdramInit();	
	
	//Setup LPC2478 LCD Controller for our specific LCD
	//lcd_config is defined in lcd/lcd_cfg.h
	lcdInit(&lcd_config); 
	
	//Turn the LCD on
	lcdTurnOn();

	//Setup touchscreen
	touch_init();
	
	
	while(1) {
		default_screen();
		non_default_screen();
	}
}


void default_screen(void) {
	char x=0, y=0, z1=0, z2=0;
	
	//Define button locations 
	//coffee machine button  
	int xc0 = 20, yc0= 40, xc1= 100, yc1= 110;
	
	//bee cam button 
	int xbc0 = 130, ybc0= 40, xbc1 = 210, ybc1 = 110;
	
	//bedroom room button 
	int xbr0 = 20, ybr0= 30, xbr1 = 100, ybr1 = 200;
	
	//lounge room blind button 
	int xlr0 = 130, ylr0= 130, xlr1 = 210, ylr1 = 200;
	
	//Draw defult home screen
	draw_default_screen();
	
	//Loop forever
	while (1) {
		int R_xplate = 5; 
		int R_touch = 0;
		int threshold = 0;
		int point_x = 0;
		int point_y = 0;
		
		//Read in X and Y coordinates
		touch_read_xy(&x,&y, &z1, &z2);
		
		// Algorithim to decide if we can pop the bubble
		//R_touch = R_xplate*x/1024*(z2/z1 - 1);
		
		//check if screen should be changed
		//scale the coordinates to the screens resolution
		point_x = x*240/255;
		point_y = y*320/255; 
		
		if (xc0 < point_x && point_x < xc1 && yc0 < point_y && point_y < yc1){
			draw_coffee_machine_screen();
			return;
		} //else if (point_x > 50) {
			//door_bell();
			//return; }
		  else if (xbr0 < point_x && point_x < xbr1 && ybr0 < point_y && point_y < ybr1) {
			more_settings_screen("BEDROOM");
			return;
		} else if (xlr0 < point_x && point_x < xlr1 && ylr0 < point_y && point_y < ylr1) {
			more_settings_screen("LOUNGE ROOM");
			return;
		} else if(xbc0 < point_x && point_x < xbc1 && ybc0 < point_y && point_y < ybc1){
			bee_cam();
			return;
		}
	}
}	


void non_default_screen(void) {
	char x=0, y=0, z1=0, z2=0;
	
	//Define the location of the 'home' button 
	int x0=70, y0=240, x1=170, y1=290;
	
	int R_xplate = 5; 
	int R_touch = 0;
	int threshold = 0;
	int point_x = 0;
	int point_y = 0;
	
	while (1) {
		//Read in X and Y coordinates
		touch_read_xy(&x,&y, &z1, &z2);
		
		// Algorithim to decide if we can pop the bubble
		R_touch = R_xplate*x/1024*(z2/z1 - 1);
		
		//check if screen should be changed
		//scale the coordinates to the screens resolution
		point_x = x*240/255;
		point_y = y*320/255; 
		
		if (x0 < point_x && point_x < x1 && y0 < point_y && point_y < y1) {
			return;
		}
	}
}
