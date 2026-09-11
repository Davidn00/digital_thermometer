#include <xc.h>
#include <stdint.h>

#include "adc0801.h"
#include "config.h"


/*
 * PIC18F4550
 *
 * ADC0801 DB0..DB7 -> RD0..RD7
 *
 * CS   -> RA3
 * WR   -> RC0
 * RD   -> RC1
 * INTR -> RB2 / INT2
 */

#define ADC_CS      LATAbits.LATA3
#define ADC_WR      LATCbits.LATC0
#define ADC_RD      LATCbits.LATC1

#define ADC_DATA    PORTD


void ADC0801_Init(void)
{
    /*
     * AN0 remains analog because it will be used
     * by the internal PIC ADC for battery measurement.
     *
     * AN1..AN12 are digital.
     */
    ADCON0 = 0x00;
    ADCON1 = 0x0E;

    /* Disable comparators */
    CMCON = 0x07;


    /* ADC0801 data bus */
    TRISD = 0xFF;


    /* ADC0801 control */
    TRISAbits.TRISA3 = 0;

    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;


    /* ADC0801 interrupt */
    TRISBbits.TRISB2 = 1;


    /*
     * Idle state.
     * All ADC control signals are active low.
     */
    ADC_CS = 1;
    ADC_WR = 1;
    ADC_RD = 1;
}


void ADC0801_StartConversion(void)
{
    ADC_CS = 0;

    ADC_WR = 0;

    __delay_us(2);

    ADC_WR = 1;
}


uint8_t ADC0801_Read(void)
{
    uint8_t value;


    ADC_CS = 0;

    ADC_RD = 0;

    __delay_us(1);

    value = ADC_DATA;

    ADC_RD = 1;


    return value;
}