#include <stdint.h>

#include "stats.h"


static uint32_t sum = 0;

static uint32_t count = 0;

static uint16_t maximum = 0;

static uint16_t minimum = 0;


void Stats_Reset(void)
{
    sum = 0;

    count = 0;

    maximum = 0;

    minimum = 0;
}


void Stats_Add(uint16_t temperature10)
{
    sum += temperature10;

    count++;


    if (count == 1)
    {
        maximum = temperature10;
        minimum = temperature10;

        return;
    }


    if (temperature10 > maximum)
    {
        maximum = temperature10;
    }


    if (temperature10 < minimum)
    {
        minimum = temperature10;
    }
}


uint16_t Stats_GetAverage(void)
{
    if (count == 0)
    {
        return 0;
    }


    return (uint16_t)(
        sum / count
    );
}


uint16_t Stats_GetMaximum(void)
{
    return maximum;
}


uint16_t Stats_GetMinimum(void)
{
    return minimum;
}


uint32_t Stats_GetCount(void)
{
    return count;
}