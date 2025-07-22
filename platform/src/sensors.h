#ifndef SENSORS_H
#define SENSORS_H

// sensors.h
//
// DHT22 driver and TEMT6000
//
// Created 22.7.25

struct dht_state {
    int last_result;
};

struct temt_state {
    int last_result;
};

struct dht_state *setup_dht();

void measure_dht();

struct temt_state *setup_temt();

void measure_temt();

#endif
