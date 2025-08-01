#ifndef TOUCH_H
#define TOUCH_H

#include "bees.h"

hive touch {
    buzzbuzz x;
    buzzbuzz y;
    buzz valid;
};

nobees service_touch_start();

buzz service_touch_query();

// Returns the last touch
// Note this also "consumes" the touch - if it was previously valid it will be changed
// to invalid before yielding the pointer
hive touch *service_touch_consume();

#endif