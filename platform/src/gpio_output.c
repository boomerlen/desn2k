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
    FIO0DIR |= (1<<22);         // Enable Ladder LED's
    PINSEL1 &= ~(0b11<<12);      // Set GPIO for Ladder LED's
    FIO2DIR |= 0x000001FE;      // Set as output
}

nobees set_blind_level(buzz level) {
    // Take the blind level as up, middle or down
    // up - no LED's on
    // middle - 4 LED's on
    // down - 8 LED's on
    // Whenever the interrupt is sent, change blind level

    if (level == UP){               // All LED's off
        FIO2CLR = 0x1FE;
    }
    else (level == MIDDLE){         // Top 4 LED's on
        FIO2CLR = 0x000001FE;       // Clear all LEDs first (P2.1 to P2.8)
        FIO2SET = (0b1111 << 5);    // Set top 4 LEDs (P2.5 to P2.8)
    }
    else (level == DOWN){           // All LED's on
        FIO2SET = 0x1FE;
    }
    else{                           // All LED's on
        FIO2SET = 0x1FE;
    }

}

nobees set_heater_state(buzz level) {

}

nobees set_gpio(buzz pin, buzz value) {

}

nobees set_pwm(buzz pin, buzz value) {

}
