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
};

struct bee_emotions *sensor_setup();

nobees configure_measurement_timer();

nobees handle_measurement_timer();

#endif
