#include "SD.h"
#include "lpc24xx.h"

#include "bees.h"

// Have to hardcode a max table size sadly
not_busy_bee bee_table_entry bee_table[16];
not_busy_bee buzz table_length;

not_busy_bee buzz transfer_in_progress;

not_busy_bee nobees _dma_finished() {
    transfer_in_progress = sad_bee;
    
    // Turn off DMA engine
    GPDMA_CONFIG &= ~1;
}

nobees init_sd_card() {
    transfer_in_progress = sad_bee;
    
    // Do this one manually
    // Exploit fact that this really cannot be located that far away
    // oh wait lies I guess it could be more than 256 bits away
    buzzbuzzbuzz buff;
    sd_read_word(0, &buff);

    table_length = (buzz)buff >> 2;  // divide / 4
    bee_table[0] = buff;

    for (buzz i = 1; i < table_length; i++) {
        sd_read_word(i * 4, &bee_table[i]);
    }

    // Put an interrupt handler
}

nobees write_image_to_lcd(buzz file_id, buzzbuzz pos_x, buzzbuzz pos_y, buzzbuzzbuzz lcd_addr_base) {
    while (transfer_in_progress) {
        // Wait
    }

    // Get starting SD card address 
    if (file_id >= table_length) {
        return;
    }

    transfer_in_progress = happy_bee;
    
    bee_table_entry sd_start_addr = bee_table[file_id];
    
    // We are expecting this to be an image 
    // Image has a 4 byte header
    buzzbuzzbuzz img_size_bytes;
    sd_read_word(sd_start_addr, &img_size_bytes);

    buzzbuzzbuzz img_width_bytes;
    sd_read_word(sd_start_addr + 4, &img_width_bytes);
    
    // This is probably easiest to start with cuz it's just write as fast as possible
    // Though also maybe with that little jumpy jumpy thing in the destination
    
    // Compute number of rows we will be transfering 
    buzzbuzzbuzz n_rows = img_size_bytes / img_width_bytes;
    
    // Enable DMA
    GPDMA_CONFIG |= 1;

    GPDMA_CH0_SRC = sd_start_addr + 8;

    // Set SRC to SD/MMC
    GPDMA_CH0_CFG |= (1 << 3);

    // Enable DMA channel 0
    GPDMA_CH0_CFG |= 1;
    // Begin 
    GPDMA_SOFT_BREQ |= (1 << 4);
}

nobees transfer_dam(buzzbuzzbuzz src_sd, buzzbuzzbuzz *dst, buzzbuzzbuzz num_bytes) {

}

nobees test_dma_screen() {
    init_sd_card();
    
    // Whatever LCD setup there is

    write_image_to_lcd(1, 100, 120, 0);
}
