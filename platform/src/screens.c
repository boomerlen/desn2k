#include "bees.h"
#include "hw/lcd/lcd_hw.h"
#include "hw/lcd/lcd_grph.h"
#include "hw/lcd/lcd_cfg.h"

nobees draw_default_screen() {
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
    lcd_putString(25, 45, (unsigned char *)"Coffee machine");
    lcd_fillcircle(60, 80, 15, DARK_GRAY);
    lcd_line(55, 60, 55, 50, DARK_GRAY);
    lcd_line(60, 60, 60, 50, DARK_GRAY);
    lcd_line(65, 60, 65, 50, DARK_GRAY);

    // Living Room Light button
    lcd_fillRect(130, 40, 210, 110, WHITE);
    lcd_drawRect(130, 40, 210, 110, BLACK);
    lcd_putString(135, 45, (unsigned char *)"Living Room");
    lcd_putString(145, 60, (unsigned char *)"light");
    lcd_fillcircle(170, 80, 12, YELLOW);
    lcd_fillRect(165, 90, 175, 100, YELLOW);

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

    // Centered analog + digital clock

    // Multiple alternating rings
    lcd_fillcircle(cx, cy, 32, BLACK);
    lcd_fillcircle(cx, cy, 28, YELLOW);
    lcd_fillcircle(cx, cy, 24, BLACK);
    lcd_fillcircle(cx, cy, 20, YELLOW);

    // White numbers only on top black ring
    lcd_fontColor(WHITE, BLACK);
    lcd_putString(cx - 4, cy - 24, (unsigned char *)"12");
    lcd_putString(cx + 20, cy - 4, (unsigned char *)"3");
    lcd_putString(cx - 4, cy + 14, (unsigned char *)"6");
    lcd_putString(cx - 24, cy - 4, (unsigned char *)"9");

    // Clock arms
    lcd_line(cx, cy, cx, cy - 10, BLACK); // hour hand
    lcd_line(cx, cy, cx + 12, cy, BLACK); // minute hand

    // Digital clock centered beside it
    lcd_fontColor(BLACK, YELLOW);
    lcd_putString(cx + 50, cy - 4, (unsigned char *)"12:45 PM");
}


nobees more_settings_screen(const char *room_name) {
    lcd_fillScreen(WHITE);
    lcd_fontColor(BLACK, WHITE);

    // Outer border
    lcd_drawRect(5, 5, DISPLAY_WIDTH - 5, DISPLAY_HEIGHT - 5, BLACK);

    // Title
    lcd_putString(70, 20, (unsigned char *)room_name);

    // Auto mode label
    lcd_putString(20, 60, (unsigned char *)"Automatic mode");

    // Toggle switch
    lcd_drawRect(150, 55, 220, 75, BLACK);
    lcd_putString(155, 60, (unsigned char *)"OFF");
    lcd_fillRect(185, 55, 220, 75, DARK_GRAY);
    lcd_fontColor(WHITE, DARK_GRAY);
    lcd_putString(190, 60, (unsigned char *)"ON");
    lcd_fontColor(BLACK, WHITE);

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
    lcd_putString(20, 200, (unsigned char *)"0%%");
    lcd_putString(200, 200, (unsigned char *)"100%%");

    // Nighttime buttons
    lcd_drawRect(70, 198, 90, 218, BLACK);  // Left arrow
    lcd_putString(75, 203, (unsigned char *)"<");

    lcd_drawRect(160, 198, 180, 218, BLACK);  // Right arrow
    lcd_putString(165, 203, (unsigned char *)">");
}

nobees draw_coffee_machine_screen() {
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

    // Guest coffee label
    lcd_putString(20, 120, (unsigned char *)"Guest Coffee");

    // ON/OFF button outline
    lcd_drawRect(135, 115, 205, 135, BLACK);
    lcd_putString(140, 120, (unsigned char *)"OFF");

    // ON bubble filled
    lcd_fillRect(175, 115, 205, 135, DARK_GRAY);
    lcd_fontColor(WHITE, DARK_GRAY);
    lcd_putString(180, 120, (unsigned char *)"ON");

    // Reset font color
    lcd_fontColor(BLACK, WHITE);

    // Draw saucer base (ellipse-style using lines)
    lcd_line(80, 260, 160, 260, BLACK);     // base line
    lcd_line(90, 255, 150, 255, BLACK);     // top of saucer

    // Cup body (simple arc and bowl)
    lcd_fillcircle(120, 230, 30, WHITE);    // cup fill
    lcd_circle(120, 230, 30, BLACK);        // cup outline

    // Coffee surface
    lcd_fillcircle(120, 230, 20, BLACK);    // black coffee fill

    // Handle (semi-ellipse on the right)
    lcd_circle(150, 230, 10, BLACK);        // cup handle outline

    // Steam (curved strokes with lines)
    lcd_line(110, 180, 115, 160, BLACK);    // lower curl
    lcd_line(115, 160, 120, 150, BLACK);
    lcd_line(120, 150, 125, 140, BLACK);

    lcd_line(125, 190, 130, 170, BLACK);    // upper curl
    lcd_line(130, 170, 135, 160, BLACK);
    lcd_line(135, 160, 140, 150, BLACK);
}


nobees door_bell() {
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

}
