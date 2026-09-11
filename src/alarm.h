#ifndef ALARM_H
#define ALARM_H

#include <stdint.h>

typedef enum
{
    ALARM_NORMAL = 0,
    ALARM_LOW,
    ALARM_HIGH

} AlarmState;


void Alarm_Init(void);

AlarmState Alarm_Update(uint16_t temperature10);

AlarmState Alarm_GetState(void);

#endif