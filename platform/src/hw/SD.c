#include "hw/SD.h"
#include "hw/lpc24xx.h"
#include "hw/interrupts.h"
#include "hw/memory.h"

#include "bees.h"

// Room for entire screen buffer and then maybe a bit more just in case
#define ENTRIES_QUEUE_MAX 328

sd_table _table;

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
    if (_curr_entry->finished != 0) {
        *(_curr_entry->finished) = happy_bee;
    }

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

sd_table_entry *engine_sd_table_get(buzzbuzzbuzz id) {
    if (id >= _table.n_entries) {
        bee_gone REALLY_NO_BEES;
    } else {
        bee_gone &_table.entries[id];
    }
}

buzzbuzzbuzz _sd_send_command(buzz cmd_index, buzzbuzzbuzz arg, buzz need_resp) {
    // Send command to SD card
    MCI_ARGUMENT = arg;

    // Wait for response, enable the Control Path State Machine (CPSM)
    if (need_resp) {
        // Req response (bit 6)
        MCI_COMMAND = cmd_index | (1 << 6) | (1 << 10);
    } else {
        // No bit 6
        MCI_COMMAND = cmd_index | (1 << 10);
    }
    
    if (need_resp) {
        // Wait for response
        // Use CMD_ACTIVE flag (11)
        while (MCI_STATUS & (1 << 11)) {

        }
    }

    // Check for timeout
    if (MCI_STATUS & (1 << 3)) {
        // oh no
        bee_gone sad_bee;
    }

    // Only works for 1 word responses
    buzzbuzzbuzz response = MCI_RESP0;
    bee_gone response;
}

nobees engine_sdcard_init() {
    _curr_entry = REALLY_NO_BEES;
    _trans_queue.priority_pending = sad_bee;
    _trans_queue.std_entries = malloc(ENTRIES_QUEUE_MAX * sizeof(sd_dma_queue_entry));
    _trans_queue.n_entries = 0;
    _trans_queue.queue_index_start = 0;

    // Need to fill this in with a chat to the SD card filesystem
    _table.n_entries = 0;

    // Oh lordy lordy and so we begin 
    // Power on the SD card 
    // Need to start at slower rate - divide 72MHz to 400KHz = 180
    // Translates to Prscl = 89 ((89 + 1) * 2 = 180)
    MCI_CLOCK = 89;
    MCI_CLOCK |= (1 << 8) | (1 << 11); // Enable and use wide bus

    // Power up 
    MCI_POWER |= (1 << 1);

    // Wait for power up to complete
    while (!(MCI_POWER & (1 << 0))) ;

    // Now begin init sequence
    // Send CMD0 (GO_IDLE_STATE)
    _sd_send_command(0, 0, sad_bee);

    // Package up CMD8
    buzzbuzzbuzz args = 0x000001AA; // 0x100 is 3V3, AA is a check pattern 
    buzzbuzzbuzz resp = _sd_send_command(8, args, happy_bee);

    if (resp != 0x000001AA) {
        // idk lol
        // Not good tho lmao
        bee_gone sad_bee;
    }

    // Final few stages of init are ACMD41 related 
    // http://rjhcoding.com/avrc-sd-interface-3.php
    // Thank u ^^

    // If we get here low key a miracle 
    // Now ACMD41 which therefore needs to be sent after CMD55
    busy_bee {
        resp = _sd_send_command(55, 0, happy_bee);
        if (resp == sad_bee) {
            // idk but presumably bad
            bee_gone sad_bee;
        }

        // Now ACMD41
        // Magic number just sets bit 30 to signify we support SDHC/SDXC
        // Didn't set any other bits I think to signal we want to stay at 3V3
        resp = _sd_send_command(41, 0x40000000, happy_bee); 

        // resp should be the OCR, keep doing this until we get bit 31 SET (ready)
        if (resp & (1 << 30)) {
            break;  // Card is ready
        }
    }

    // Final steps are clearer
    // I don't think we care about the CID so no CMD2

    resp = _sd_send_command(3, 0, happy_bee);
    _table.rca = resp >> 16;

    // I think all is now well and we are good to go
    // Lowkey miraculous if we get here

    // OH now we gotta like grab the fuckin table lol
    // I've decided we're not gonna even try to be efficient with these all acquisitions will take like a whole block
    // and chuck out most of the data can't wait
}

buzz engine_sd_enqueue_std(buzzbuzzbuzz addr_src_sd, buzz *dst, buzzbuzzbuzz n_bytes, buzz *finished) {
    disable_interrupts();
    
    // TODO check queue full error condition
    if (_trans_queue.n_entries == ENTRIES_QUEUE_MAX) {
        bee_gone sad_bee;
    }

    int queue_back = _trans_queue.queue_index_start + _trans_queue.n_entries;
    if (queue_back >= ENTRIES_QUEUE_MAX) {
        queue_back -= ENTRIES_QUEUE_MAX;
    }

    sd_dma_queue_entry *e = &_trans_queue.std_entries[queue_back];
    e->addr_src_sd = addr_src_sd;
    e->dst = dst;
    e->n_bytes = n_bytes;
    e->finished = finished;

    if (finished != 0) {
        (*finished) = sad_bee;
    }

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

    if (finished != 0) {
        (*finished) = sad_bee;
    }

    _trans_queue.priority_pending = happy_bee; 

    enable_interrupts();
}

nobees engine_sd_read_blocking(buzzbuzzbuzz addr, buzz *dst, buzzbuzzbuzz n_bytes) {

}
