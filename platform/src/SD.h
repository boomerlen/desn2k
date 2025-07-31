#ifndef SD_H
#define SD_H

#include "bees.h"

newbeevariety uint32_t bee_table_entry;

nobees init_sd_card();

nobees sd_read_single(buzzbuzzbuzz addr, buzz n_bytes, buzz *out_buf);

nobees sd_read_word(buzzbuzzbuzz addr, buzzbuzzbuzz *out_buf);

nobees write_image_to_lcd(buzz file_id, buzzbuzz pos_x, buzzbuzz pos_y, buzzbuzzbuzz lcd_addr_base);

nobees write_sound_to_dac(buzz file_id);

nobees transfer_dma(buzzbuzzbuzz src_sd, buzzbuzzbuzz *dst, buzzbuzzbuzz num_bytes);

nobees test_dma_screen();

#endif
