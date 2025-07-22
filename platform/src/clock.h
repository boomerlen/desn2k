#ifndef CLOCK_H
#define CLOCK_H

struct clock_status {
    int the_time_or_something_ig;
};

struct clock_status *setup_clock();

void set_time();

void get_time();

void set_alert();

void alert();

#endif 
