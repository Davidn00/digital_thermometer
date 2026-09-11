#include <xc.h>
#include <stdint.h>

#include "timer.h"
#include "seg7.h"


static volatile uint32_t system_millis = 0;


static void Timer0_Reload(void)
{
    /*
     * Fosc = 20 MHz
     *
     * Instruction clock = 5 MHz
     *
     * Timer0 prescaler = 1:8
     *
     * Timer tick = 1.6 us
     *
     * 625 ticks = 1 ms
     *
     * 65536 - 625 = 64911 = 0xFD8F
     */

    TMR0H = 0xFD;
    TMR0L = 0x8F;
}


void Timer_Init(void)
{
    /*
     * Timer0:
     *
     * 16-bit
     * Internal instruction clock
     * Prescaler 1:8
     */

    T0CON = 0x82;

    Timer0_Reload();

    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
}


void Timer_1ms_ISR(void)
{
    if (INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = 0;

        Timer0_Reload();

        system_millis++;
    }


    /*
     * 7-segment multiplexing at approximately 500 Hz.
     */
    static uint8_t mux_divider = 0;

    mux_divider++;

    if (mux_divider >= 2)
    {
        mux_divider = 0;

        Seg7_Refresh();
    }
}


uint32_t Timer_Millis(void)
{
    uint32_t value;

    uint8_t gie_state;


    gie_state = INTCONbits.GIE;

    INTCONbits.GIE = 0;

    value = system_millis;

    INTCONbits.GIE = gie_state;


    return value;
}


uint8_t Timer_Elapsed(
    uint32_t now,
    uint32_t *last,
    uint32_t period
)
{
    if ((uint32_t)(now - *last) >= period)
    {
        *last = now;

        return 1;
    }

    return 0;
}