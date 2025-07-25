#include "coffee.h"
#include "clock.h"
#include "sensors.h"
#include "doorbell.h"
#include "gpio_output.h"

#include "bees.h"

struct bee_first_order_system {
    struct bee_emotions *bee_bee_bee;
    struct clock_status *bee_oclock;
} bee_system;

queen_bee {
    bee_system.bee_bee_bee = sensor_setup();	

    bee_system.bee_oclock = setup_clock();

    setup_bee_envigorator();

    doorbell_setup();
    
    configure_gpio();

    busy_bee {
        // Done
    }

	return happy_bee;
}
