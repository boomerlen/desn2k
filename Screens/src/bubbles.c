#include "bubbles.h"
#include "lpc24xx.h"   
#include "lcd/lcd_hw.h"
#include "lcd/lcd_grph.h"
#include "lcd/lcd_cfg.h"
#include "lcd/sdram.h"
#include "delay.h"
#include <stdlib.h>


void draw_bee(int x, int y, int scale) {
    // Bee body segments
    lcd_fillcircle(x, y, scale, YELLOW);                    // segment 1
    lcd_fillcircle(x + scale, y, scale, BLACK);             // segment 2
    lcd_fillcircle(x + 2 * scale, y, scale, YELLOW);        // segment 3
    lcd_fillcircle(x + 3 * scale, y, scale - 2, BLACK);     // head

    // Eyes
    lcd_fillcircle(x + 3 * scale - 2, y - 3, scale / 4, LIGHT_GRAY);
    lcd_fillcircle(x + 3 * scale + 2, y - 3, scale / 4, LIGHT_GRAY);

    // Wings
    lcd_fillcircle(x + scale, y - scale - 1, scale / 2, LIGHT_GRAY);
    lcd_fillcircle(x + 2 * scale, y - scale - 1, scale / 2, LIGHT_GRAY);
}

void draw_default_screen(void) {
	  int cx = 80, cy = 280;
	  lcd_fillScreen(YELLOW);
    lcd_fontColor(BLACK, YELLOW);

    // Outer black border
    lcd_drawRect(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1, BLACK);

    // Header
    lcd_putString(70, 10, (unsigned char *)"THE BEE HIVE");
    lcd_drawRect(180, 10, 230, 30, BLACK);
    lcd_putString(185, 15, (unsigned char *)"THERE");

    // Coffee machine button
    lcd_fillRect(20, 40, 100, 110, WHITE);
    lcd_drawRect(20, 40, 100, 110, BLACK);
    lcd_putString(30, 45, (unsigned char *)"Coffee");
		lcd_putString(30, 60, (unsigned char *)"machine");
    lcd_fillcircle(60, 90, 15, DARK_GRAY);

    // bee cam button
    lcd_fillRect(130, 40, 210, 110, WHITE);
    lcd_drawRect(130, 40, 210, 110, BLACK);
    lcd_putString(135, 45, (unsigned char *)"Bee Cam");
		draw_bee(160, 80, 8);

    // Bed Room Blind button
    lcd_fillRect(20, 130, 100, 200, WHITE);
    lcd_drawRect(20, 130, 100, 200, BLACK);
    lcd_putString(25, 135, (unsigned char *)"Bed Room");
    lcd_putString(30, 150, (unsigned char *)"Blind");
    lcd_drawRect(35, 165, 85, 190, DARK_GRAY);
    lcd_line(60, 165, 60, 190, DARK_GRAY);

    // Lounge Room Blind button
    lcd_fillRect(130, 130, 210, 200, WHITE);
    lcd_drawRect(130, 130, 210, 200, BLACK);
    lcd_putString(135, 135, (unsigned char *)"Lounge Room");
    lcd_putString(145, 150, (unsigned char *)"Blind");
    lcd_drawRect(145, 165, 195, 190, DARK_GRAY);
    lcd_line(170, 165, 170, 190, DARK_GRAY);

		draw_bee(40, 276, 15);

    // Digital clock centered beside it
    lcd_fontColor(BLACK, YELLOW);
    lcd_putString(cx + 50, cy - 4, (unsigned char *)"12:45 PM");
}


void more_settings_screen(const char *room_name) {
    lcd_fillScreen(WHITE);
    lcd_fontColor(BLACK, WHITE);

    // Outer border
    lcd_drawRect(5, 5, DISPLAY_WIDTH - 5, DISPLAY_HEIGHT - 5, BLACK);

    // Title
    lcd_putString(80, 40, (unsigned char *)room_name);

    // Daytime brightness
    lcd_putString(20, 110, (unsigned char *)"Daytime brightness");
    lcd_putString(20, 130, (unsigned char *)"0%%");
    lcd_putString(200, 130, (unsigned char *)"100%%");

    // Daytime buttons
    lcd_drawRect(70, 128, 90, 148, BLACK);  // Left arrow
    lcd_putString(75, 133, (unsigned char *)"<");

    lcd_drawRect(160, 128, 180, 148, BLACK);  // Right arrow
    lcd_putString(165, 133, (unsigned char *)">");

    // Nighttime brightness
    lcd_putString(20, 180, (unsigned char *)"Nighttime brightness");
    lcd_putString(20, 200, (unsigned char *)"0%");
    lcd_putString(200, 200, (unsigned char *)"100%");

    // Nighttime buttons
    lcd_drawRect(70, 198, 90, 218, BLACK);  // Left arrow
    lcd_putString(75, 203, (unsigned char *)"<");

    lcd_drawRect(160, 198, 180, 218, BLACK);  // Right arrow
    lcd_putString(165, 203, (unsigned char *)">");
		
		// HOME button at the bottom
		lcd_drawRect(70, 240, 170, 290, BLACK);      // button outline
		lcd_putString(110, 260, (unsigned char *)"HOME");
		lcd_fontColor(WHITE,BLACK);

}

void draw_coffee_machine_screen(void) {
    int cx = 120, cy = 220;  
	  lcd_fillScreen(WHITE);
    lcd_fontColor(BLACK, WHITE);

    // Outer border
    lcd_drawRect(5, 5, DISPLAY_WIDTH - 5, DISPLAY_HEIGHT - 5, BLACK);

    // Title
    lcd_putString(80, 20, (unsigned char *)"COFFEE MACHINE");

    // First coffee
    lcd_putString(20, 60, (unsigned char *)"First Coffee");
    lcd_putString(140, 60, (unsigned char *)"7:00AM ON");

    // Second coffee
    lcd_putString(20, 90, (unsigned char *)"Second Coffee");
    lcd_putString(140, 90, (unsigned char *)"11:00AM ON");

    // Reset font color
    lcd_fontColor(BLACK, WHITE);
	
		draw_bee(80, 150, 20);
		
		// HOME button at the bottom
		lcd_drawRect(70, 240, 170, 290, BLACK);      // button outline
		lcd_putString(110, 260, (unsigned char *)"HOME");
		lcd_fontColor(WHITE,BLACK);

}


void door_bell(void) {
		// Center point for bell
	int center_x = 120;
	int center_y = 140;
	
	//Setup external SDRAM. Used for Frame Buffer
	sdramInit();	
	
	//Setup LPC2478 LCD Controller for our specific LCD
	//lcd_config is defined in lcd/lcd_cfg.h
	lcdInit(&lcd_config); 
	
	//Turn the LCD on
	lcdTurnOn();
	
	// Clear screen with dark blue
	lcd_fillScreen(WHITE);


	// Bell dome (semi-circle top of bell)
	lcd_fillcircle(center_x, center_y - 20, 20, YELLOW);

	// Bell base
	lcd_fillRect(center_x - 20, center_y - 20, center_x + 20, center_y + 10, YELLOW);

	// Bell clapper
	lcd_fillcircle(center_x, center_y + 15, 5, DARK_GRAY);

	// BUZZ ALERT label (bigger by writing twice offset)
	lcd_fontColor(BLACK, WHITE);
	lcd_putString(80, 30, "BUZZ ALERT!");
	lcd_putString(81, 31, "BUZZ ALERT!");  // Offset for bold effect

	// === Bees (reused style) ===

		// Top Left
		lcd_fillcircle(60, 70, 8, YELLOW);
		lcd_fillcircle(68, 70, 8, BLACK);
		lcd_fillcircle(76, 70, 8, YELLOW);
		lcd_fillcircle(84, 70, 6, BLACK);
		lcd_fillcircle(82, 67, 2, LIGHT_GRAY);
		lcd_fillcircle(86, 67, 2, LIGHT_GRAY);
		lcd_fillcircle(70, 60, 4, LIGHT_GRAY);
		lcd_fillcircle(75, 60, 4, LIGHT_GRAY);

		// Top Right
		lcd_fillcircle(180, 70, 8, YELLOW);
		lcd_fillcircle(172, 70, 8, BLACK);
		lcd_fillcircle(164, 70, 8, YELLOW);
		lcd_fillcircle(156, 70, 6, BLACK);
		lcd_fillcircle(158, 67, 2, LIGHT_GRAY);
		lcd_fillcircle(154, 67, 2, LIGHT_GRAY);
		lcd_fillcircle(170, 60, 4, LIGHT_GRAY);
		lcd_fillcircle(165, 60, 4, LIGHT_GRAY);

		// Bottom Left
		lcd_fillcircle(60, 200, 8, YELLOW);
		lcd_fillcircle(68, 200, 8, BLACK);
		lcd_fillcircle(76, 200, 8, YELLOW);
		lcd_fillcircle(84, 200, 6, BLACK);
		lcd_fillcircle(82, 197, 2, LIGHT_GRAY);
		lcd_fillcircle(86, 197, 2, LIGHT_GRAY);
		lcd_fillcircle(70, 190, 4, LIGHT_GRAY);
		lcd_fillcircle(75, 190, 4, LIGHT_GRAY);

		// Bottom Right
		lcd_fillcircle(180, 200, 8, YELLOW);
		lcd_fillcircle(172, 200, 8, BLACK);
		lcd_fillcircle(164, 200, 8, YELLOW);
		lcd_fillcircle(156, 200, 6, BLACK);
		lcd_fillcircle(158, 197, 2, LIGHT_GRAY);
		lcd_fillcircle(154, 197, 2, LIGHT_GRAY);
		lcd_fillcircle(170, 190, 4, LIGHT_GRAY);
		lcd_fillcircle(165, 190, 4, LIGHT_GRAY);
		
		// HOME button at the bottom
		lcd_drawRect(70, 240, 170, 290, BLACK);      // button outline
		lcd_putString(110, 260, (unsigned char *)"HOME");
		lcd_fontColor(WHITE,BLACK);

	}

void bee_cam(void) {
    lcd_fillScreen(BLACK);

    // Bold bee watch label
    lcd_fontColor(BLACK, YELLOW);
    lcd_putString(80, 30, "BEE WATCH MODE!");
    lcd_putString(81, 31, "BEE WATCH MODE!");

    // Draw bees of different sizes and positions
    draw_bee(40, 70, 6);     // Top Left - medium
    draw_bee(150, 60, 8);    // Top Right - large
    draw_bee(30, 190, 5);    // Bottom Left - small
    draw_bee(160, 150, 7);   // Bottom Right - medium-large
    draw_bee(90, 100, 4);    // Center Left - small
    draw_bee(120, 250, 9);   // Center Bottom - extra large
    draw_bee(20, 70, 6);     // Top Left - medium
    draw_bee(150, 7, 8);    // Top Right - large
    draw_bee(40, 190, 5);    // Bottom Left - small
    draw_bee(160, 19, 7);   // Bottom Right - medium-large
    draw_bee(90, 100, 4);    // Center Left - small
    draw_bee(10, 200, 9);   // Center Bottom - extra large
		draw_bee(150, 7, 8);    // Top Right - large
    draw_bee(40, 190, 10);    // Bottom Left - small
    draw_bee(12, 180, 10);   // Bottom Right - medium-large
    draw_bee(50, 50, 10);    // Center Left - small
    draw_bee(180, 200, 10);   // Center Bottom - extra large
		draw_bee(0, 0, 10);    // Bottom Left - small
    draw_bee(50, 50, 10);   // Bottom Right - medium-large
    draw_bee(50, 100, 10);    // Center Left - small
    draw_bee(240, 240, 10);   // Center Bottom - extra large
    draw_bee(120, 180, 10);    // Center Left - small
    draw_bee(120, 240, 10);   // Center Bottom - extra large
		
    // HOME button
    lcd_fillRect(70, 240, 170, 290, WHITE);
    lcd_fontColor(BLACK, WHITE);
    lcd_putString(110, 260, (unsigned char *)"HOME");
}


