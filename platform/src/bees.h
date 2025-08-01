#ifndef BEE_H
#define BEE_H

#include <stdint.h>

#define buzz uint8_t 
#define buzzbuzz uint16_t
#define buzzbuzzbuzz uint32_t
#define nobees void
#define happy_bee 1
#define sad_bee 0

#define queen_bee int main()
#define busy_bee while (1)
#define hive struct
#define beenum enum
#define newbeevariety typedef
#define not_busy_bee static

#define bee_is_buzzy(BYTE, BIT) ((BYTE) & (1 << (BIT)))

#define worker_bee(X) void (*X)(void)

typedef union bee_cast {
    uint8_t bytes[4];
    uint32_t word;
} bee_cast;

#endif
