#ifndef SD_H
#define SD_H

#include "bees.h"

// Depends on SDRAM being setup!

#define MAX_SD_ENTRIES 8

newbeevariety hive sd_table_entry {
    buzzbuzzbuzz addr;
    buzzbuzzbuzz len;
} sd_table_entry;

newbeevariety hive sd_table {
    buzzbuzz rca;
    buzzbuzz n_entries;
    sd_table_entry entries[MAX_SD_ENTRIES];
} sd_table;

nobees engine_sdcard_init();

sd_table_entry *engine_sd_table_get(buzzbuzzbuzz id);

// 1 == failure, 0 == all good
buzz engine_sd_enqueue_std(buzzbuzzbuzz addr_src_sd, buzz *dst, buzzbuzzbuzz n_bytes, buzz *finished);

nobees engine_sd_enqueue_priority(buzzbuzzbuzz addr_src_sd, buzz *dst, buzzbuzzbuzz n_bytes, buzz *finished);

nobees engine_sd_read_blocking(buzzbuzzbuzz addr, buzz *dst, buzzbuzzbuzz n_bytes);

#endif
