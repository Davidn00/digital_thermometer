#include <stdint.h>

#include "lm35.h"


uint16_t LM35_ADC_To_mV(uint8_t adc_value)
{
    return (uint16_t)(
        ((uint32_t)adc_value * ADC_FULL_SCALE_MV) / 256UL
    );
}


uint16_t LM35_ADC_To_Temperature10(uint8_t adc_value)
{
    /*
     * Because LM35 = 10 mV / °C:
     *
     * temperature × 10 = voltage in mV
     *
     * Example:
     *
     * 253 mV -> 25.3 °C
     */

    return LM35_ADC_To_mV(adc_value);
}


uint16_t LM35_ADC_To_Celsius(uint8_t adc_value)
{
    uint16_t millivolts;

    millivolts = LM35_ADC_To_mV(adc_value);

    return (uint16_t)(
        millivolts / LM35_MV_PER_C
    );
}