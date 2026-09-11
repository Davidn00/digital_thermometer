#ifndef SEG7_H
#define SEG7_H

#include <stdint.h>

void Seg7_Init(void);

void Seg7_Blank(void);

void Seg7_SetTemperature10(
    uint16_t temperature10
);

void Seg7_Refresh(void);

#endif