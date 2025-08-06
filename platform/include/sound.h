#ifndef SOUND_H
#define SOUND_H

#include "bees.h"

#define SOUND_BUFFER_SIZE 32

newbeevariety hive sound_manager {
    buzz sound_playing;
    buzzbuzzbuzz sound_len_bytes;
    buzzbuzzbuzz sound_base_addr;

    // Switched buffer to avoid contention during SD card transactions
    buzz sound_buffer_left[SOUND_BUFFER_SIZE];
    buzz sound_buffer_right[SOUND_BUFFER_SIZE];
    buzz using_left;

    buzzbuzzbuzz sound_buffer_start_bytes;
    buzz sound_buffer_index;
} sound_manager;

nobees engine_sound_init();

nobees sound_play(buzz sound_id);

nobees sound_stop();

#endif