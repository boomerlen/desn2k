#ifndef SD_H
#define SD_H

#include "bees.h"

// Depends on SDRAM being setup!

nobees engine_sdcard_init();

// 1 == failure, 0 == all good
buzz engine_sd_enqueue_std(buzzbuzzbuzz addr_src_sd, buzz *dst, buzzbuzzbuzz n_bytes, buzz *finished);

nobees engine_sd_enqueue_priority(buzzbuzzbuzz addr_src_sd, buzz *dst, buzzbuzzbuzz n_bytes, buzz *finished);

nobees engine_sd_read_single(buzzbuzzbuzz addr, buzz *dst, buzzbuzzbuzz n_bytes);

#endif
