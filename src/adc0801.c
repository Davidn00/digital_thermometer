#include <xc.h>
#include <stdint.h>

#include "adc0801.h"

void ADC0801_Init(void)
{
    /*
     * D0-D7 del ADC0801 conectados a PORTD.
     */
    TRISD = 0xFF;

    /*
     * INTR del ADC0801 conectado a RB0/INT0.
     */
    TRISBbits.TRISB0 = 1;
}

void ADC0801_StartConversion(void)
{
    /*
     * La lógica de WR se implementará
     * de acuerdo con el esquemático de Proteus.
     */
}

uint8_t ADC0801_Read(void)
{
    return PORTD;
}

uint8_t ADC0801_IsReady(void)
{
    /*
     * INTR es activo en nivel bajo.
     */
    return (PORTBbits.RB0 == 0);
}