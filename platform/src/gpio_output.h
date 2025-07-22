#ifndef GPIO_OUTPUT_H
#define GPIO_OUTPUT_H

// gpio_output.h
//
// Responsible for managing Heater, Coffee Machine and Blind level (both of which are represented by GPIO LEDs
//
// Created 22.7.25

#include <stdint.h>


// Add extra status if needed, I think only coffee machine will require it though 
struct coffee_status {
    uint8_t active;
};

struct coffee_status *setup_coffee();

void enable_coffee();

void coffee_timer_finished();

void setup_blind();

void set_blind_level(int level);

void setup_heater();

void set_heater_state();

#endif

