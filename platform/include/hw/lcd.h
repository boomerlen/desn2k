#ifndef LCD_H
#define LCD_H

#include "bees.h"

struct lcd_state {
    bool lock;
};

hive image {
    buzz width_pixels;
    buzz height_pixels;
    buzz total_size_pixels;
    buzz total_size_bytes; // Doule total_size_pixels
    buzzbuzzbuzz base_addr;
}; // 64-bits


nobees write_image();

#endif
