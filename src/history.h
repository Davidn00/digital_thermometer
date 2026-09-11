#ifndef HISTORY_H
#define HISTORY_H

#include <stdint.h>

typedef struct
{
    uint16_t sequence;

    uint16_t temperature10;

    uint8_t battery;

    uint8_t alarm;

    uint8_t dac_level;

} HistoryRecord;


void History_Init(void);

uint8_t History_Log(
    uint16_t temperature10,
    uint8_t battery,
    uint8_t alarm,
    uint8_t dac_level
);

uint8_t History_GetLatest(
    HistoryRecord *record
);

uint16_t History_GetCount(void);

#endif