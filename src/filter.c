#include <stdint.h>

#include "filter.h"

static uint16_t sample_sum = 0;
static uint8_t sample_count = 0;
static uint8_t filter_ready = 0;
static uint8_t filtered_value = 0;


void Filter_Reset(void)
{
    sample_sum = 0;
    sample_count = 0;
    filter_ready = 0;
    filtered_value = 0;
}


void Filter_AddSample(uint8_t sample)
{
    sample_sum += sample;

    sample_count++;

    if (sample_count >= FILTER_SAMPLES)
    {
        filtered_value = sample_sum >> 7;

        sample_sum = 0;
        sample_count = 0;

        filter_ready = 1;
    }
}


uint8_t Filter_IsReady(void)
{
    return filter_ready;
}


uint8_t Filter_GetAverage(void)
{
    filter_ready = 0;

    return filtered_value;
}