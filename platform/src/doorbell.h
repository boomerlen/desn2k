#ifndef DOORBELL_GPIO_H
#define DOORBELL_GPIO_H

// doorbell_gpio.h
// 
// Manages doorbell activation and timing
//
// Created 22.7.25

#include "bees.h"

// If this doesn't run async (just plays music) then can just be the one action

nobees doorbell_setup();

nobees event_doorbell_pressed();

#endif
