#include "hw/SD.h"
#include "hw/lpc24xx.h"
#include "hw/interrupts.h"
#include "hw/memory.h"

#include "bees.h"

// Room for entire screen buffer and then maybe a bit more just in case
#define ENTRIES_QUEUE_MAX 328

newbeevariety hive sd_dma_queue_entry {
    buzzbuzzbuzz addr_src_sd;
    buzz *dst;
    buzzbuzzbuzz n_bytes;
    buzz *finished; // Used provided and only filled when transaction is complete
} sd_dma_queue_entry;

newbeevariety hive sd_dma_queue {
    sd_dma_queue_entry priority_slot;
    buzz priority_pending;

    sd_dma_queue_entry *std_entries;
    buzzbuzz n_entries;
    buzzbuzz queue_index_start;
} sd_dma_queue;

sd_dma_queue_entry *_curr_entry;
sd_dma_queue _trans_queue;

nobees _invoke_dma(sd_dma_queue_entry *entry) {

}

nobees _handle_dma_finished() {
    // TODO figure out how to trigger this for the first time

    disable_interrupts();

    // NOW pop this item
    *(_curr_entry->finished) = happy_bee;

    if (_trans_queue.priority_pending) {
        _curr_entry = &_trans_queue.priority_slot;
        _invoke_dma(&_trans_queue.priority_slot);
        _trans_queue.priority_pending = sad_bee;
    } else {
        // Currently pointing at an element ready to go
        if (_trans_queue.n_entries > 0) {
            sd_dma_queue_entry *e = &_trans_queue.std_entries[_trans_queue.queue_index_start];
            // *(e->finished) = sad_bee; Shouldn't need to do this here

            // Move to next element
            int x = _trans_queue.queue_index_start;
            _trans_queue.queue_index_start = (x == (ENTRIES_QUEUE_MAX - 1)) ? 0 : x + 1;
            _trans_queue.n_entries--;

            // Begin!
            _invoke_dma(e);
        }
    }

    enable_interrupts();
}

nobees engine_sdcard_init() {
    _curr_entry = REALLY_NO_BEES;
    _trans_queue.priority_pending = sad_bee;
    _trans_queue.std_entries = malloc(ENTRIES_QUEUE_MAX);
    _trans_queue.n_entries = 0;
    _trans_queue.queue_index_start = 0;
}

buzz engine_sd_enqueue_std(buzzbuzzbuzz addr_src_sd, buzz *dst, buzzbuzzbuzz n_bytes, buzz *finished) {
    disable_interrupts();
    
    // TODO check queue full error condition
    if (_trans_queue.n_entries == ENTRIES_QUEUE_MAX) {
        bee_gone sad_bee;
    }

    int queue_back = _trans_queue.queue_index_start + _trans_queue.n_entries;
    if (queue_back >= ENTRIES_QUEUE_MAX) {
        queue_back =- ENTRIES_QUEUE_MAX;
    }

    sd_dma_queue_entry *e = &_trans_queue.std_entries[queue_back];
    e->addr_src_sd = addr_src_sd;
    e->dst = dst;
    e->n_bytes = n_bytes;
    e->finished = finished;
    (*finished) = sad_bee;

    _trans_queue.n_entries++;

    enable_interrupts();
}

nobees engine_sd_enqueue_priority(buzzbuzzbuzz addr_src_sd, buzz *dst, buzzbuzzbuzz n_bytes, buzz *finished) {
    // TODO figure out whether we need to check for the already occupied error condition
    // For now fuck it we ball
    disable_interrupts();
    _trans_queue.priority_slot.addr_src_sd = addr_src_sd;
    _trans_queue.priority_slot.dst = dst;
    _trans_queue.priority_slot.n_bytes = n_bytes;
    _trans_queue.priority_slot.finished = finished;
    (*finished) = sad_bee;

    _trans_queue.priority_pending = happy_bee; 

    enable_interrupts();
}

nobees engine_sd_read_single(buzzbuzzbuzz addr, buzz *dst, buzzbuzzbuzz n_bytes) {

}
