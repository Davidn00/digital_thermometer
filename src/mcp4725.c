#include <stdint.h>

#include "mcp4725.h"
#include "i2c.h"
#include "config.h"


static const uint16_t dac_levels[DAC_LEVEL_COUNT] =
{
    0,
    1024,
    2048,
    3072,
    4095
};


void MCP4725_Init(void)
{
    MCP4725_SetCode(0);
}


uint8_t MCP4725_SetCode(uint16_t code)
{
    uint8_t success;


    if (code > 4095)
    {
        code = 4095;
    }


    I2C_Start();


    success = I2C_WriteByte(
        (MCP4725_ADDRESS << 1) | 0
    );


    if (!success)
    {
        I2C_Stop();

        return 0;
    }


    /*
     * Write DAC register.
     *
     * Command = 0x40
     */
    success = I2C_WriteByte(0x40);


    if (!success)
    {
        I2C_Stop();

        return 0;
    }


    success = I2C_WriteByte(
        (uint8_t)(code >> 4)
    );


    if (!success)
    {
        I2C_Stop();

        return 0;
    }


    success = I2C_WriteByte(
        (uint8_t)((code & 0x0F) << 4)
    );


    I2C_Stop();


    return success;
}


uint8_t MCP4725_SetLevel(uint8_t level)
{
    if (level >= DAC_LEVEL_COUNT)
    {
        level = 0;
    }


    return MCP4725_SetCode(
        dac_levels[level]
    );
}


uint16_t MCP4725_GetLevelCode(uint8_t level)
{
    if (level >= DAC_LEVEL_COUNT)
    {
        level = 0;
    }


    return dac_levels[level];
}