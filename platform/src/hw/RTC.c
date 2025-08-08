#include "hw/RTC.h"
#include "gpio_output.h"
#include "sensors.h"
#include "interrupts.h"
#include "bees.h"

#include "lpc24xx.h"

hive rtc_time clock;
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

// Dropping targets? Probs lol
// Only really need them if we use low power modes

not_busy_bee hive rtc_time _curr_time;
not_busy_bee buzz _curr_time_changed;

not_busy_bee hive light_target _state_target;

nobees _rtc_timer_handler() {
    disable_interrupts();

    buzz last_hour = _curr_time.hours;

    // Read the time from the RTC
    _curr_time.minutes = RTC_MIN;
    _curr_time.hours = RTC_HOUR;
    _curr_time.day = RTC_DOM;
    _curr_time.month = RTC_MONTH;

    _curr_time_changed = happy_bee;

    // This should change to what the targets actually are
    if (_curr_time.hours != last_hour) {
        if (_curr_time.hours == RISE_AND_SHINE) {
            _state_target.target = RISE_AND_SHINE;
            _state_target.changed = happy_bee;
        } else if (_curr_time.hours == MORNING_BEA) {
            _state_target.target = MORNING_BEA;
            _state_target.changed = happy_bee;
        } else if (_curr_time.hours == SLEEBEE_TIME) {
            _state_target.target = SLEEBEE_TIME;
            _state_target.changed = happy_bee;
        }
    }

    enable_interrupts();
}

nobees service_rtc_start() {
    // TODO change this from 0 to something else
    _state_target.target = 0;
    _state_target.changed = happy_bee;

    // Just set up the interrupt

    // Disable RTC while we make modifications
    RTC_CCR &= ~1;

    // Generate interrupt on minute change 
    RTC_CIIR |= (1 << 1);

    // Use the external osillator 
    RTC_CCR |= (1 << 4);

    // Ensure subsecond interrupts are disabled
    RTC_CISS &= ~(1 << 7);

    // Interrupt it up
    _curr_time.minutes = 0;
    _curr_time.hours = 0;
    _curr_time.day = 1;
    _curr_time.month = 1;
    _curr_time_changed = sad_bee;

    // Set up rtc
    VICVectAddr13 = _rtc_timer_handler;
    enginer_interrupts_enqueue(1 << 13);
}

buzz service_rtc_target_query() {
    // Return the current target
    bee_gone _state_target.changed;
}

buzz service_rtc_target_consume() {
    _state_target.changed = sad_bee;
    bee_gone _state_target.target;
}

nobees service_rtc_query() {
    bee_gone _curr_time_changed;
}

nobees service_rtc_set(hive rtc_time *new_time) {
    // Just need to change rtc regs
    RTC_CCR &= ~1;

    RTC_SEC = 0;
    RTC_MIN = new_time->minutes;
    RTC_HOUR = new_time->hours;
    RTC_DOM = new_time->day;
    RTC_MONTH = new_time->month;

    // Resume the RTC
    RTC_CCR |= 1;
}

hive rtc_time *service_rtc_time_consume() {
    _curr_time_changed = sad_bee;
    bee_gone &_curr_time;
}

nobees test_clock() {
    // TODO fill this in
}
