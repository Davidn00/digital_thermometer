#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

#define FILTER_SAMPLES 128U

void Filter_Reset(void);

void Filter_AddSample(uint8_t sample);

uint8_t Filter_IsReady(void);

uint8_t Filter_GetAverage(void);

#endif