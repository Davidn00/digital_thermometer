#include <stdint.h>

#include "pcf8574.h"
#include "i2c.h"
#include "config.h"


void PCF8574_Init(void)
{
    PCF8574_Write(0x08);
}


uint8_t PCF8574_Write(uint8_t value)
{
    uint8_t result;


    I2C_Start();


    result = I2C_WriteByte(
        (PCF8574_ADDRESS << 1) | 0
    );


    if (!result)
    {
        I2C_Stop();

        return 0;
    }


    result = I2C_WriteByte(value);


    I2C_Stop();


    return result;
}