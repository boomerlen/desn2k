#ifndef DOORBELL_GPIO_H
#define DOORBELL_GPIO_H

// doorbell_gpio.h
// 
// Manages doorbell activation and timing
//
// Created 22.7.25

#include <stdint.h>

struct doorbell_status {
    uint8_t active;
};

struct doorbell_status *init();

void doorbell_pressed();

void doorbell_timer_finished();


#endif
