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

}

nobees set_heater_state(buzz level) {

}

nobees set_gpio(buzz pin, buzz value) {

}

nobees set_pwm(buzz pin, buzz value) {

}
