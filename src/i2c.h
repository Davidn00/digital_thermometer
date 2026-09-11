#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void I2C_Init(void);

void I2C_Start(void);

void I2C_Stop(void);

uint8_t I2C_WriteByte(uint8_t data);

uint8_t I2C_ReadByte(uint8_t ack);

#endif