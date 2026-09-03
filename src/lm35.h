#ifndef LM35_H
#define LM35_H

#include <stdint.h>

/*
 * With VREF/2 = 2.5 V, the ADC0801 full-scale span is 5.0 V.
 * Therefore:
 *
 *     LSB = 5000 mV / 256
 *
 * LM35 output = 10 mV / °C.
 */
#define ADC_FULL_SCALE_MV 5000UL
#define LM35_MV_PER_C     10UL

uint16_t LM35_ADC_To_mV(uint8_t adc_value);
uint16_t LM35_ADC_To_Celsius(uint8_t adc_value);

#endif
