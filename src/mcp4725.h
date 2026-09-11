#ifndef MCP4725_H
#define MCP4725_H

#include <stdint.h>

void MCP4725_Init(void);

uint8_t MCP4725_SetCode(uint16_t code);

uint8_t MCP4725_SetLevel(uint8_t level);

uint16_t MCP4725_GetLevelCode(uint8_t level);

#endif