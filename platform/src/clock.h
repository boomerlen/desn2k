#ifndef CLOCK_H
#define CLOCK_H

#include "bees.h"

beenum BuzzHour {
    RISE_AND_SHINE = 7,
    MORNING_BEA = 11,
    SLEEBEE_TIME = 18
};

hive clock_status {
    buzz minutes;
    buzz hours;
    buzz day; // 1 -> 31
    buzz month; // 1 -> 12
    buzz current_hour_alarm;
};

hive clock_status *setup_clock(hive bee_emotions *bee_target_hive);

nobees calibrate_targets();

nobees set_time(buzz sec, buzz min, buzz hour, buzz day, buzz month);

nobees alert();

nobees reset_alert();

nobees test_clock();

#endif 
