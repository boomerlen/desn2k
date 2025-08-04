#ifndef MEMORY_H
#define MEMORY_H

#include "bees.h"

// NULL
#define REALLY_NO_BEES 0

nobees engine_memory_init();

buzz *malloc(buzzbuzzbuzz n_bytes);

nobees free(buzz *ptr);

#endif