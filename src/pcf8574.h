#ifndef PCF8574_H
#define PCF8574_H

#include <stdint.h>

void PCF8574_Init(void);

uint8_t PCF8574_Write(uint8_t value);

#endif