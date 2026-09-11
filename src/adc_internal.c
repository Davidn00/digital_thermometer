#include <xc.h>
#include <stdint.h>

#include "adc_internal.h"
#include "config.h"


void ADC_Internal_Init(void)
{
    /*
     * AN0 = analog
     * AN1..AN12 = digital
     */
    ADCON1 = 0x0E;


    /*
     * Right justified
     *
     * Acquisition = 12 TAD
     *
     * ADC clock = Fosc/64
     */
    ADCON2 = 0xAE;


    /*
     * Channel AN0
     */
    ADCON0bits.CHS = 0;

    ADCON0bits.ADON = 1;
}


uint16_t ADC_Internal_Read(uint8_t channel)
{
    uint16_t result;


    ADCON0bits.CHS = channel;

    __delay_us(20);


    ADCON0bits.GO = 1;

    while (ADCON0bits.GO)
    {
        ;
    }


    result =
        ((uint16_t)ADRESH << 8) |
        ADRESL;


    return result;
}


uint8_t Battery_ReadPercent(void)
{
    uint16_t raw;

    uint32_t pin_mv;

    uint32_t battery_mv;

    uint32_t percent;


    raw = ADC_Internal_Read(0);


    /*
     * PIC ADC reference = VDD = 5V
     */
    pin_mv =
        ((uint32_t)raw * 5000UL) / 1023UL;


    /*
     * Recover actual battery voltage
     */
    battery_mv =
        (pin_mv * BATTERY_DIVIDER_NUM) /
        BATTERY_DIVIDER_DEN;


    if (battery_mv <= BATTERY_MIN_MV)
    {
        return 0;
    }


    if (battery_mv >= BATTERY_MAX_MV)
    {
        return 100;
    }


    percent =
        ((battery_mv - BATTERY_MIN_MV) * 100UL) /
        (BATTERY_MAX_MV - BATTERY_MIN_MV);


    return (uint8_t)percent;
}