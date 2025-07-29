#ifndef SENSORS_H
#define SENSORS_H

// sensors.h
//
// DHT22 driver and TEMT6000
//
// Created 22.7.25

#include "bees.h"

struct bee_emotions {
    buzzbuzz temp_result;
    buzzbuzz light_result;

    buzz changed;

    buzzbuzz temp_target;
    buzzbuzz light_target;
};

// Standard levels here 
beenum target_temp {
    TEMP_SLEEP_BEE = 22,
    TEMP_BUZZY_BEE = 32
};

beenum target_light {
    LIGHT_SLEEP_BEE = 22,
    LIGHT_BUZZY_BEE = 98
};

struct bee_emotions *sensor_setup();

nobees configure_measurement_timer();

nobees handle_measurement_timer();

#endif
