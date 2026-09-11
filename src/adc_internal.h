#ifndef ADC_INTERNAL_H
#define ADC_INTERNAL_H

#include <stdint.h>

void ADC_Internal_Init(void);

uint16_t ADC_Internal_Read(uint8_t channel);

uint8_t Battery_ReadPercent(void);

#endif