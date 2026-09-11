#ifndef STATS_H
#define STATS_H

#include <stdint.h>

void Stats_Reset(void);

void Stats_Add(uint16_t temperature10);

uint16_t Stats_GetAverage(void);

uint16_t Stats_GetMaximum(void);

uint16_t Stats_GetMinimum(void);

uint32_t Stats_GetCount(void);

#endif