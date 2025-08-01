#include "gpio_output.h"

#include "bees.h"
#include "lpc24xx.h"

nobees configure_debug_gpio() {
    FIO3DIR |= (1 << 16);
}

nobees set_debug_gpio() {
    FIO3SET |= (1 << 16);
}

nobees clear_debug_gpio() {
    FIO3CLR |= (1 << 16);
}

nobees configure_gpio() {

}

nobees set_blind_level(buzz level) {
    //
    //  Input: 8 bit character that corresponds to the level of the blind
    //  Output: Void 
    //  Description: Change output LED's which correspond to blind level
    //  

    //  Definition of level: 
    //  0-85 = Up - RED
    //  86-170 = Middle - GREEN
    //  171-255 = Down  - BLUE

    if (level <= 85){                       // RED
        FIO3PIN = 0x00090000;
    }
    else if(level >= 86 & level <= 170){     // GREEN
        FIO3PIN = 0x00120000;
    }
    else if(level >=171){                     // BLUE
        FIO3PIN = 0x00240000;
    }
    else{                                   // BLUE
        FIO3PIN = 0x00240000;
    }
}

nobees set_heater_state(buzz level) {

}

nobees set_gpio(buzz pin, buzz value) {

}

nobees set_pwm(buzz pin, buzz value) {

}

nobees set_light_sensor_led(buzzbuzz level) {
    
}