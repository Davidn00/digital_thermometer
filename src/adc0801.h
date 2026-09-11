#ifndef ADC0801_H
#define ADC0801_H

#include <stdint.h>

void ADC0801_Init(void);
void ADC0801_StartConversion(void);
uint8_t ADC0801_Read(void);

#endif