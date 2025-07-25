#include "sensors.h"

#include "bees.h"

struct bee_emotions how_are_the_bees = {
    .temp_result = sad_bee,
    .light_result = sad_bee,
    .changed = happy_bee
};

struct bee_emotions *sensor_setup() {
    // Do setup
    

    // Must return this reference otherwise the handler can't figure things out
    return &how_are_the_bees;
}

nobees configure_measurement_timer() {
    // Fill this in
}

nobees handle_measurement_timer() {
    // Fill this in
}

