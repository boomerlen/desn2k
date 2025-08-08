#ifndef LIGHT_H
#define LIGHT_H

// sensors.h
//
// Light sensor
//
// Created 22.7.25

#include "bees.h"

// Standard levels here 
newbeevariety beenum target_light {
    LIGHT_SLEEP_BEE = 22,
    LIGHT_BUZZY_BEE = 98
} target_light;

newbeevariety hive bee_emotions {
    buzzbuzz light_result;
    buzz changed;

    target_light target;
} bee_emotions;

// Not completely sure how target changing will work
// Maybe just don't actually do it?
// Especially since RTC alerts aren't going to be a thing?
// Unsure actually if target code should really live here

// Configure system + setup internal structures
nobees service_light_start();

// Checks if there's an unused light value
buzz service_light_query();

// Yields light result, sets changed to sad_bee
buzzbuzz service_light_consume();


#endif
