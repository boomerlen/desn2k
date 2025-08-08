#include "sound.h"
#include "hw/lpc24xx.h"
#include "hw/SD.h"
#include "hw/interrupts.h"
#include "hw/memory.h"  

not_busy_bee sound_manager _sm;

nobees _sound_timer_handler();

nobees engine_sound_init() {
    _sm.sound_len_bytes = 0;
    _sm.sound_buffer_start_bytes = 0;
    _sm.sound_buffer_index = 0;
    _sm.sound_playing = sad_bee;

    // Going to use Timer 3 for this (I think)
    // Prescale by 8 then count to 1125 for 8kHz
    // According to https://eleccelerator.com/avr-timer-calculator/ 
    // Thank u ^^^
    T3PR = 8;
    T3MR0 = 1125;

    // Stop and interrupt on match
    T3MCR |= (1 << 2) | (1 << 0);

    // Put handler in right spot
    VICVectAddr27 = _sound_timer_handler;

    // Put interrupt in right spot 
    engine_interrupts_enqueue(1 << 27);

    // Configure the DAC
    PINSEL1 = PINSEL1 & ~(1 << 20) | (1 << 21);
	FIO0DIR |= 0x4000000; // set up pin 0.26 to output 
}

nobees _sound_timer_handler() {
    // Check if we're finished
    buzzbuzzbuzz this_spot = _sm.sound_buffer_start_bytes + _sm.sound_buffer_index;
    if (this_spot == _sm.sound_len_bytes) {
        _sm.sound_playing = sad_bee;

        // Turn off timer
        // Don't really need to reset
        T3TCR = 0;

        bee_gone;
    }

    // Shoot this one to the DAC
    if (_sm.using_left) {
        DACR = _sm.sound_buffer_left[_sm.sound_buffer_index] << 6;
    } else {
        DACR = _sm.sound_buffer_right[_sm.sound_buffer_index] << 6;
    }

    // Check if we need to swap buffers + repopulate
    _sm.sound_buffer_index++;
    if (_sm.sound_buffer_index == SOUND_BUFFER_SIZE) {
        // Hopefully other buffer is ready to go lmao
        // Not going to check just like surely
        // Could confirm this actually using the finished flag 
        // But this really shouldn't be an issue

        // Does mean we have to refresh this one
        buzz *to_fill = _sm.using_left ? _sm.sound_buffer_left : _sm.sound_buffer_right;
        buzzbuzzbuzz next_buff_start = _sm.sound_base_addr + _sm.sound_buffer_start_bytes + 2 * SOUND_BUFFER_SIZE;
        engine_sd_enqueue_priority(next_buff_start, to_fill, SOUND_BUFFER_SIZE, 0);

        // Swap buffers
        _sm.using_left = !_sm.using_left;
        
        // Reset + update all indices
        _sm.sound_buffer_index = 0;
        _sm.sound_buffer_start_bytes += SOUND_BUFFER_SIZE;
    }
}

nobees sound_play(buzz sound_id) {
    // WOWOWOWOOW
    // Firstly just don't do anything if we're already playing a song 
    if (_sm.sound_playing) {
        bee_gone;
    }

    sd_table_entry *e = engine_sd_table_get(sound_id);
    if (e == REALLY_NO_BEES) {
        bee_gone;
    }

    // Setup sound manager
    _sm.sound_len_bytes = e->len;
    _sm.sound_base_addr = e->addr;    

    // Put indices in start positions
    _sm.sound_buffer_start_bytes = 0;
    _sm.sound_buffer_index = 0;

    // Populate both buffers
    _sm.using_left = happy_bee;

    // Busy wait on this (why not)
    // Or should I?
    // Easy fix if we notice latency here
    buzz finished_second;
    engine_sd_enqueue_std(_sm.sound_base_addr, _sm.sound_buffer_left, SOUND_BUFFER_SIZE, 0);
    engine_sd_enqueue_std(_sm.sound_base_addr + SOUND_BUFFER_SIZE, _sm.sound_buffer_right, SOUND_BUFFER_SIZE, &finished_second);

    while (finished_second == sad_bee) {

    }

    // All ready!
    // Enable timer
    T3TCR = 1;
}

nobees sound_stop() {
    // Just stop the timer
    T3TCR = 0;
    _sm.sound_playing = sad_bee;
}