#include <stdint.h>
#include "lm35.h"

uint16_t LM35_ADC_To_mV(uint8_t adc_value)
{
    /*
     * ADC0801 code is proportional to 0..5 V.
     * Integer arithmetic is used to avoid floating point.
     */
    return (uint16_t)(((uint32_t)adc_value * ADC_FULL_SCALE_MV) / 256UL);
}

uint16_t LM35_ADC_To_Celsius(uint8_t adc_value)
{
    uint16_t millivolts = LM35_ADC_To_mV(adc_value);

    return (uint16_t)(millivolts / LM35_MV_PER_C);
}
