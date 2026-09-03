#include <xc.h>
#include <stdint.h>
#include "adc0801.h"
#include "config.h"

#define ADC_CS      LATAbits.LATA3
#define ADC_WR      LATCbits.LATC0
#define ADC_RD      LATAbits.LATA5

#define ADC_DATA    PORTD

void ADC0801_Init(void)
{
    /*
     * Disable the PIC's internal analog functions and comparators.
     * The ADC0801 is an external ADC; PORTA/PORTB must operate digitally.
     */
    ADCON0 = 0x00;
    ADCON1 = 0x0F;
    CMCON  = 0x07;

    /* ADC0801 data bus: input to PIC */
    TRISD = 0xFF;

    /* ADC0801 control outputs */
    TRISAbits.TRISA3 = 0;    // CS
    TRISCbits.TRISC0 = 0;    // WR
    TRISAbits.TRISA5 = 0;    // RD

    /* ADC0801 INTR input */
    TRISBbits.TRISB2 = 1;

    /* Idle state: CS/RD/WR inactive (active-low signals) */
    ADC_CS = 1;
    ADC_RD = 1;
    ADC_WR = 1;
}

void ADC0801_StartConversion(void)
{
    /*
     * CS and WR must be low to request a conversion.
     * Conversion starts after WR returns high.
     */
    ADC_CS = 0;
    ADC_WR = 0;
    __delay_us(2);
    ADC_WR = 1;
}

uint8_t ADC0801_Read(void)
{
    uint8_t value;

    /*
     * Both CS and RD low enable the ADC0801 output latch.
     * The data becomes valid shortly after RD falls.
     */
    ADC_CS = 0;
    ADC_RD = 0;

    __delay_us(1);
    value = ADC_DATA;

    ADC_RD = 1;

    return value;
}
