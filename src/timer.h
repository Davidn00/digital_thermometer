#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void Timer_Init(void);

void Timer_1ms_ISR(void);

uint32_t Timer_Millis(void);

uint8_t Timer_Elapsed(
    uint32_t now,
    uint32_t *last,
    uint32_t period
);

#endif