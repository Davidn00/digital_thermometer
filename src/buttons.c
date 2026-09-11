#include <xc.h>
#include <stdint.h>

#include "buttons.h"


#define BUTTON_MODE_PIN     PORTBbits.RB0
#define BUTTON_SELECT_PIN   PORTBbits.RB1


void Buttons_Init(void)
{
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;


    /*
     * Enable PORTB weak pull-ups.
     */
    INTCON2bits.RBPU = 0;
}


static uint8_t DebounceButton(
    uint8_t raw,
    uint8_t *stable,
    uint8_t *counter
)
{
    if (raw == *stable)
    {
        *counter = 0;

        return 0;
    }


    (*counter)++;


    if (*counter >= 3)
    {
        *counter = 0;

        *stable = raw;


        /*
         * Event generated when button becomes pressed.
         */
        if (raw == 0)
        {
            return 1;
        }
    }


    return 0;
}


uint8_t Buttons_Update(void)
{
    static uint8_t mode_stable = 1;
    static uint8_t mode_counter = 0;

    static uint8_t select_stable = 1;
    static uint8_t select_counter = 0;


    uint8_t events = BUTTON_NONE;


    if (DebounceButton(
            BUTTON_MODE_PIN,
            &mode_stable,
            &mode_counter))
    {
        events |= BUTTON_MODE;
    }


    if (DebounceButton(
            BUTTON_SELECT_PIN,
            &select_stable,
            &select_counter))
    {
        events |= BUTTON_SELECT;
    }


    return events;
}