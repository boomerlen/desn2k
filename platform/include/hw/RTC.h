#ifndef CLOCK_H
#define CLOCK_H

#include "bees.h"

beenum BuzzHour {
    RISE_AND_SHINE = 7,
    MORNING_BEA = 11,
    SLEEBEE_TIME = 18
};

hive rtc_time {
    buzz minutes;
    buzz hours;
    buzz day; // 1 -> 31
    buzz month; // 1 -> 12
};

hive light_target {
    buzz target;
    buzz changed;
};

nobees service_rtc_start();

nobees service_rtc_query();

nobees service_rtc_set(hive rtc_time *new_time);

// Returns a pointer to the time and marks it as read
hive rtc_time *service_rtc_time_consume();

buzz service_rtc_target_query();

// Returns the new target and marks it as seen (not yielded again)
buzz service_rtc_target_consume();

#endif 
