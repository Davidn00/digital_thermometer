#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>

uint8_t EEPROM_WriteBytes(
    uint16_t address,
    const uint8_t *data,
    uint8_t length
);

uint8_t EEPROM_ReadBytes(
    uint16_t address,
    uint8_t *data,
    uint8_t length
);

#endif