#ifndef MEMORY_H
#define MEMORY_H

#include "bees.h"

// NULL
#define REALLY_NO_BEES 0

// Dummy memory allocation which does work but also doesn't
nobees engine_memory_init();

// Will indeed give you some valid memory you can use 
buzz *malloc(buzzbuzzbuzz n_bytes);

// Doesn't do anything lol
// So basically we always leak memory but should be fine
// Maybe one day it'll do something 
// Low key would then have to worry about like memory segmentation and
// would probably want full on virtualisation which I'm not doing
nobees free(buzz *ptr);

#endif