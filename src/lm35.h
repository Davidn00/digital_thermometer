#ifndef LM35_H
#define LM35_H

#include <stdint.h>

#define ADC_FULL_SCALE_MV  5000UL
#define LM35_MV_PER_C      10UL

uint16_t LM35_ADC_To_mV(uint8_t adc_value);

uint16_t LM35_ADC_To_Temperature10(uint8_t adc_value);

uint16_t LM35_ADC_To_Celsius(uint8_t adc_value);

#endif