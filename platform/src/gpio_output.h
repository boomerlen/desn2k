#ifndef GPIO_OUTPUT_H
#define GPIO_OUTPUT_H

// gpio_output.h
//
// Responsible for managing Heater, Coffee Machine and Blind level (both of which are represented by GPIO LEDs
//
// Created 22.7.25

#include "bees.h"

// Debug GPIO is always going to be P3.16 - LED1 Red
nobees configure_debug_gpio();

nobees set_debug_gpio();
nobees clear_debug_gpio();

nobees configure_gpio();

nobees set_blind_level(buzz level);

nobees set_heater_state(buzz on);

nobees set_gpio(buzz pin, buzz value);

nobees set_pwm(buzz pin, buzz value);

#endif

