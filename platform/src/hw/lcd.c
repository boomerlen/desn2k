#include "bees.h"
#include "hw/lcd.h"
#include "hw/lcd/lcd_cfg.h"

nobees engine_lcd_init() {
    // In lcd_cfg.h
    lcdInit(&lcd_config);

    lcdTurnOn();
}
