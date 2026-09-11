#include <xc.h>
#include <stdint.h>

#include "seg7.h"


#define SEG_A       LATAbits.LATA1
#define SEG_B       LATAbits.LATA2

#define SEG_C       LATCbits.LATC2
#define SEG_D_MASK  0x20U
#define SEG_E       LATCbits.LATC6
#define SEG_F       LATCbits.LATC7

#define SEG_G       LATEbits.LATE0
#define SEG_DP      LATEbits.LATE1


#define DIG1        LATBbits.LATB3
#define DIG2        LATBbits.LATB4
#define DIG3        LATBbits.LATB5
#define DIG4        LATEbits.LATE2


#define DIGIT_OFF   1
#define DIGIT_ON    0


static uint8_t display_digits[4];

static uint8_t decimal_mask = 0;

static uint8_t current_digit = 0;


static const uint8_t segment_table[10] =
{
    0x3F, /* 0 */
    0x06, /* 1 */
    0x5B, /* 2 */
    0x4F, /* 3 */
    0x66, /* 4 */
    0x6D, /* 5 */
    0x7D, /* 6 */
    0x07, /* 7 */
    0x7F, /* 8 */
    0x6F  /* 9 */
};


static void Seg7_DisableDigits(void)
{
    DIG1 = DIGIT_OFF;
    DIG2 = DIGIT_OFF;
    DIG3 = DIGIT_OFF;
    DIG4 = DIGIT_OFF;
}


static void Seg7_EnableDigit(uint8_t digit)
{
    switch (digit)
    {
        case 0:
            DIG1 = DIGIT_ON;
            break;

        case 1:
            DIG2 = DIGIT_ON;
            break;

        case 2:
            DIG3 = DIGIT_ON;
            break;

        case 3:
            DIG4 = DIGIT_ON;
            break;

        default:
            break;
    }
}


static void Seg7_WriteSegments(uint8_t pattern)
{
    /*
     * Common anode:
     *
     * segment ON  = 0
     * segment OFF = 1
     */


    SEG_A = !(pattern & 0x01U);

    SEG_B = !(pattern & 0x02U);

    SEG_C = !(pattern & 0x04U);


    /*
     * SEG_D -> RC5
     *
     * RC5 is manipulated through LATC directly because
     * the XC8 PIC18F4550 device header does not expose
     * LATC5 as a bitfield.
     */
    if (pattern & 0x08U)
    {
        LATC |= SEG_D_MASK;
    }
    else
    {
        LATC &= (uint8_t)~SEG_D_MASK;
    }


    SEG_E = !(pattern & 0x10U);

    SEG_F = !(pattern & 0x20U);

    SEG_G = !(pattern & 0x40U);

    SEG_DP = !(pattern & 0x80U);
}


void Seg7_Init(void)
{
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;

    TRISCbits.TRISC2 = 0;
    TRISC &= (uint8_t)~SEG_D_MASK;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;

    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;


    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;


    Seg7_DisableDigits();

    Seg7_WriteSegments(0);


    Seg7_Blank();
}


void Seg7_Blank(void)
{
    display_digits[0] = 10;
    display_digits[1] = 10;
    display_digits[2] = 10;
    display_digits[3] = 10;

    decimal_mask = 0;
}


void Seg7_SetTemperature10(
    uint16_t temperature10
)
{
    uint16_t whole;

    uint8_t decimal;


    whole = temperature10 / 10U;

    decimal = temperature10 % 10U;


    display_digits[0] =
        (whole >= 100U)
        ? (uint8_t)(whole / 100U)
        : 10U;


    display_digits[1] =
        (whole >= 10U)
        ? (uint8_t)((whole / 10U) % 10U)
        : 10U;


    display_digits[2] =
        (uint8_t)(whole % 10U);


    display_digits[3] =
        decimal;


    /*
     * Decimal point after digit 3.
     *
     * Example:
     *
     * 25.3
     *
     * digit 2 = 5
     */
    decimal_mask = 0x04;
}


void Seg7_Refresh(void)
{
    uint8_t digit;

    uint8_t pattern;


    Seg7_DisableDigits();


    digit =
        display_digits[current_digit];


    if (digit < 10)
    {
        pattern =
            segment_table[digit];
    }
    else
    {
        pattern = 0;
    }


    if (decimal_mask &
        (1U << current_digit))
    {
        pattern |= 0x80;
    }


    Seg7_WriteSegments(pattern);


    Seg7_EnableDigit(current_digit);


    current_digit++;


    if (current_digit >= 4)
    {
        current_digit = 0;
    }
}