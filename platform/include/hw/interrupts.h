#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "bees.h"

// Add a new interrupt sourcee
// Pass in (1 << n) where n is the interrupt number
nobees engine_interrupts_enqueue(buzzbuzzbuzz new_int);

// Disables interrupts (use sparingly!)
nobees disable_interrupts();

// Re-enables interrupts
nobees enable_interrupts();

// Setup function 
nobees engine_interrupts_init();

#endif
