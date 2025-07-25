#ifndef GPIO_OUTPUT_H
#define GPIO_OUTPUT_H

// gpio_output.h
//
// Responsible for managing Heater, Coffee Machine and Blind level (both of which are represented by GPIO LEDs
//
// Created 22.7.25

#include "bees.h"

nobees configure_gpio();

nobees set_blind_level(buzz level);

nobees set_heater_state(buzz on);

#endif

