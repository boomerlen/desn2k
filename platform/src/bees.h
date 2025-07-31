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

#define bee_is_buzzy(BYTE, BIT) ((BYTE) & (1 << (BIT)))

#define worker_bee(X) void (*X)(void)

#endif
