#include <stdint.h>

#include "history.h"
#include "eeprom.h"
#include "config.h"


#define HEADER_SIZE 8U


static uint16_t next_index = 0;

static uint16_t sequence = 0;

static uint16_t record_count = 0;


static void History_SaveHeader(void)
{
    uint8_t header[HEADER_SIZE];


    header[0] = HISTORY_MAGIC;

    header[1] = HISTORY_VERSION;


    header[2] =
        (uint8_t)(next_index >> 8);

    header[3] =
        (uint8_t)next_index;


    header[4] =
        (uint8_t)(sequence >> 8);

    header[5] =
        (uint8_t)sequence;


    header[6] =
        (uint8_t)(record_count >> 8);

    header[7] =
        (uint8_t)record_count;


    EEPROM_WriteBytes(
        0x0000,
        header,
        HEADER_SIZE
    );
}


void History_Init(void)
{
    uint8_t header[HEADER_SIZE];


    if (!EEPROM_ReadBytes(
            0x0000,
            header,
            HEADER_SIZE))
    {
        next_index = 0;
        sequence = 0;
        record_count = 0;

        History_SaveHeader();

        return;
    }


    if (
        header[0] != HISTORY_MAGIC ||
        header[1] != HISTORY_VERSION
    )
    {
        next_index = 0;
        sequence = 0;
        record_count = 0;

        History_SaveHeader();

        return;
    }


    next_index =
        ((uint16_t)header[2] << 8) |
        header[3];


    sequence =
        ((uint16_t)header[4] << 8) |
        header[5];


    record_count =
        ((uint16_t)header[6] << 8) |
        header[7];


    if (next_index >= HISTORY_MAX_RECORDS)
    {
        next_index = 0;
    }


    if (record_count > HISTORY_MAX_RECORDS)
    {
        record_count = HISTORY_MAX_RECORDS;
    }
}


uint8_t History_Log(
    uint16_t temperature10,
    uint8_t battery,
    uint8_t alarm,
    uint8_t dac_level
)
{
    uint8_t data[8];

    uint16_t address;


    data[0] = HISTORY_MAGIC;


    sequence++;

    data[1] =
        (uint8_t)(sequence >> 8);

    data[2] =
        (uint8_t)sequence;


    data[3] =
        (uint8_t)(temperature10 >> 8);

    data[4] =
        (uint8_t)temperature10;


    data[5] = battery;

    data[6] = alarm;

    data[7] = dac_level;


    address =
        HISTORY_EEPROM_BASE +
        (next_index * 8U);


    if (!EEPROM_WriteBytes(
            address,
            data,
            8))
    {
        return 0;
    }


    next_index++;

    if (next_index >= HISTORY_MAX_RECORDS)
    {
        next_index = 0;
    }


    if (record_count < HISTORY_MAX_RECORDS)
    {
        record_count++;
    }


    History_SaveHeader();


    return 1;
}


uint8_t History_GetLatest(
    HistoryRecord *record
)
{
    uint16_t index;

    uint16_t address;

    uint8_t data[8];


    if (record_count == 0)
    {
        return 0;
    }


    if (next_index == 0)
    {
        index = HISTORY_MAX_RECORDS - 1;
    }
    else
    {
        index = next_index - 1;
    }


    address =
        HISTORY_EEPROM_BASE +
        (index * 8U);


    if (!EEPROM_ReadBytes(
            address,
            data,
            8))
    {
        return 0;
    }


    if (data[0] != HISTORY_MAGIC)
    {
        return 0;
    }


    record->sequence =
        ((uint16_t)data[1] << 8) |
        data[2];


    record->temperature10 =
        ((uint16_t)data[3] << 8) |
        data[4];


    record->battery = data[5];

    record->alarm = data[6];

    record->dac_level = data[7];


    return 1;
}


uint16_t History_GetCount(void)
{
    return record_count;
}