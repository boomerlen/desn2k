#ifndef SYSTEM_H
#define SYSTEM_H

#include "sensors.h"
#include "lcd.h"

struct system_state {
    int crazy;
    struct dht_state *state_dht;
    struct lcd_state *state_lcd;
};

void enter_mode_away();

void enter_mode_there();

void override_temp(int temp);

void override_blinds(int blinds);

void override_coffee(bool on);

void override_date();

#endif


