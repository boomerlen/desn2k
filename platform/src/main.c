#include "coffee.h"
#include "RTC.h"
#include "interrupts.h"
#include "sensors.h"
#include "doorbell.h"
#include "gpio_output.h"

#include "bees.h"
// Optionally define one of these to launch in a debug mode 
//#define DBT_INT
//#define DBG_CLOCK
//#define DBG_SENSORS

queen_bee {

    #ifdef DBG_CLOCK
    test_clock();
    #elif DBG_INT
    test_int();
    #else
    bee_manager_execute();
    #endif
    return happy_bee;
}

