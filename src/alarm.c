#include <xc.h>
#include <stdint.h>

#include "alarm.h"
#include "config.h"


#define LED_BLUE    LATBbits.LATB6
#define LED_RED     LATBbits.LATB7


static AlarmState alarm_state = ALARM_NORMAL;


void Alarm_Init(void)
{
    TRISBbits.TRISB6 = 0;
    TRISBbits.TRISB7 = 0;


    LED_BLUE = 0;
    LED_RED = 0;


    alarm_state = ALARM_NORMAL;
}


AlarmState Alarm_Update(uint16_t temperature10)
{
    switch (alarm_state)
    {
        case ALARM_LOW:

            if (temperature10 >= ALARM_BLUE_OFF_X10)
            {
                alarm_state = ALARM_NORMAL;
            }

            break;


        case ALARM_HIGH:

            if (temperature10 <= ALARM_RED_OFF_X10)
            {
                alarm_state = ALARM_NORMAL;
            }

            break;


        default:

            if (temperature10 <= ALARM_BLUE_ON_X10)
            {
                alarm_state = ALARM_LOW;
            }
            else if (temperature10 >= ALARM_RED_ON_X10)
            {
                alarm_state = ALARM_HIGH;
            }
            else
            {
                alarm_state = ALARM_NORMAL;
            }

            break;
    }


    LED_BLUE =
        (alarm_state == ALARM_LOW);

    LED_RED =
        (alarm_state == ALARM_HIGH);


    return alarm_state;
}


AlarmState Alarm_GetState(void)
{
    return alarm_state;
}