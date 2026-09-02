#ifdef RCONbits
#warning "RCONbits es un macro conocido por IntelliSense"
#endif

#include <xc.h>
#include <stdint.h>

#include "adc0801.h"

#define _XTAL_FREQ 20000000UL

#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF

volatile uint8_t adc_value = 0;
volatile uint8_t adc_data_ready = 0;

void Interrupt_Init(void)
{
    RCONbits.IPEN = 1;

    INTCONbits.INT0IF = 0;
    INTCON2bits.INTEDG0 = 0;
    INTCONbits.INT0IE = 1;

    INTCONbits.GIEH = 1;
}

void __interrupt(high_priority) ISR(void)
{
    if (INTCONbits.INT0IE && INTCONbits.INT0IF)
    {
        INTCONbits.INT0IF = 0;

        adc_value = ADC0801_Read();
        adc_data_ready = 1;
    }
}

void main(void)
{
    ADC0801_Init();

    Interrupt_Init();

    while (1)
    {
        if (adc_data_ready)
        {
            adc_data_ready = 0;

            /*
             * Procesamiento de la lectura ADC.
             */
        }
    }
}