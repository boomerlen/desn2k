#ifndef SCREENS_H
#define SCREENS_H

#include "bees.h"

nobees draw_default_screen();

nobees draw_coffee_machine_screen();

nobees more_settings_screen(const char *room_name);

nobees door_bell();

nobees bee_cam();

nobees non_default_screen();

#endif