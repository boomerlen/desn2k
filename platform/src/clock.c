#include "clock.h"
#include "sensors.h"
#include "bees.h"

#include "lpc24xx.h"

hive clock_status clock;
hive bee_emotions *bee_status;

// Important registers
// CCR - 0xE002 40008
//      0: CLKEN set to 1 to enable
//      4: CLKSRC set to 1 to use the external RTC oscillator
// CIR - 0xE002 400C
//      1: IMMIN set to 0 to generate an interrupt on minute chages
//      Set all other bits to 1 to mask
// CISS - 0xE002 4040
//      7: SubSecEna to 0 to disable subsecond interrupts
// AMR - 0xE002 4010
//      2: AMRHOUR set to 0 to check for alerts
//      Set rest to 1 to disable
// CTIME0 - 0xE002 4014 actual time register
//      5:0: seconds
//      13:8: minutes 
//      20:16: hours 
//      26:24: day of the week 
// CTIME1 - 0xE002 4018 second time register 
//      4:0: day of the month 
//      11:8: month 
//      27:16: year
// ALHOUR - 0xE002 4068 hour alarm register

// HUGO GHUO HUGO fill in calibrate targets and add targets to the target hive thing


hive clock_status *setup_clock(hive bee_emotions *bee_target_hive) {
    bee_status = bee_target_hive;

    // Disable the RTC while we make modifications if it isn't already 
    RTC_CCR &= ~1; 

    // Should first check if we have an alarm set already before we change it
    if (!bee_is_buzzy(RTC_AMR, 2)) {
        // Configure the RTC alarm 
        reset_alert();
    }

    RTC_CCR |= (1 << 4);
    RTC_CISS &= ~(1 << 7);

    // Get the current hour alarm out of the clock now (may as well)
    clock.current_hour_alarm = RTC_ALHOUR;

    // Start / resume the clock!
    RTC_CCR |= 1;

    return &clock;
}

nobees calibrate_targets() {
    // Get the time lmao 
    if (RTC_HOUR < RISE_AND_SHINE || RTC_HOUR >= SLEEBEE_TIME) {
        // Night
        // Equality to be useful in the interrupt case
        bee_status->temp_target = TEMP_SLEEP_BEE;
        bee_status->light_target = LIGHT_SLEEP_BEE;
    } else {
        // Day
        bee_status->temp_target = TEMP_BUZZY_BEE;
        bee_status->light_target = LIGHT_BUZZY_BEE;
    }
}

// Should probs call calibrate after this in auto mode!
nobees set_time(buzz sec, buzz min, buzz hour, buzz day, buzz month) {
    // Disable timer
    RTC_CCR &= ~1;

    // Set everything
    RTC_SEC = sec;
    RTC_MIN = min;
    RTC_HOUR = hour;
    RTC_DOM = day;
    RTC_MONTH = month;

    // Resume!
    RTC_CCR |= 1;
}

nobees reset_alert() {
    // Overwrite whole alert setup
    // Use RISE_AND_SHINE
    RTC_AMR = 0 | (1 << 2);
    RTC_ALHOUR = RISE_AND_SHINE;
}

nobees alert() {
    // Next one!
    calibrate_targets();
    if (clock.current_hour_alarm == RISE_AND_SHINE) {
        clock.current_hour_alarm = MORNING_BEA;
        
        // Call a whole heap of stuff

    } else if (clock.current_hour_alarm == MORNING_BEA) {
        clock.current_hour_alarm = SLEEBEE_TIME;

    } else {
        clock.current_hour_alarm = RISE_AND_SHINE;

    }
}

nobees test_clock() {
    hive bee_emotions test_emotions = {
        .temp_result = 10,
        .light_result = 6,
        .changed = sad_bee,
        .temp_target = 11,
        .light_target = 7
    };
    hive clock_status *stat = setup_clock(&test_emotions); 

    test_emotions.temp_result = 11;

    reset_alert();

    test_emotions.temp_result = 12;

    // Ideally check the alerts now
    set_time(45, 59, 6, 1, 1);

    test_emotions.temp_result = 13;

    // Should see an alert go off soon
    busy_bee {
        if (clock.current_hour_alarm != RISE_AND_SHINE) {
            break;
        }
    }

    // Can only get here if an alarm went off
    test_emotions.temp_result = 15;

    busy_bee {

    }
}
