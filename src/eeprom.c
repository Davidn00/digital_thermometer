#include <xc.h>
#include <stdint.h>

#include "config.h"
#include "eeprom.h"
#include "i2c.h"

#define EEPROM_PAGE_SIZE 64U


uint8_t EEPROM_WriteBytes(
    uint16_t address,
    const uint8_t *data,
    uint8_t length
)
{
    uint8_t chunk;

    uint8_t page_remaining;

    uint8_t success;


    while (length > 0)
    {
        page_remaining =
            EEPROM_PAGE_SIZE -
            (address & 0x3F);


        chunk =
            (length < page_remaining)
            ? length
            : page_remaining;


        I2C_Start();


        success = I2C_WriteByte(
            (EEPROM_24LC256_ADDRESS << 1) | 0
        );


        if (!success)
        {
            I2C_Stop();

            return 0;
        }


        success = I2C_WriteByte(
            (uint8_t)(address >> 8)
        );


        if (!success)
        {
            I2C_Stop();

            return 0;
        }


        success = I2C_WriteByte(
            (uint8_t)address
        );


        if (!success)
        {
            I2C_Stop();

            return 0;
        }


        for (uint8_t i = 0; i < chunk; i++)
        {
            if (!I2C_WriteByte(data[i]))
            {
                I2C_Stop();

                return 0;
            }
        }


        I2C_Stop();


        /*
         * EEPROM write cycle.
         */
        __delay_ms(6);


        address += chunk;

        data += chunk;

        length -= chunk;
    }


    return 1;
}


uint8_t EEPROM_ReadBytes(
    uint16_t address,
    uint8_t *data,
    uint8_t length
)
{
    uint8_t success;


    I2C_Start();


    success = I2C_WriteByte(
        (EEPROM_24LC256_ADDRESS << 1) | 0
    );


    if (!success)
    {
        I2C_Stop();

        return 0;
    }


    if (!I2C_WriteByte(
            (uint8_t)(address >> 8)))
    {
        I2C_Stop();

        return 0;
    }


    if (!I2C_WriteByte(
            (uint8_t)address))
    {
        I2C_Stop();

        return 0;
    }


    /*
     * Repeated START.
     */
    I2C_Start();


    if (!I2C_WriteByte(
            (EEPROM_24LC256_ADDRESS << 1) | 1))
    {
        I2C_Stop();

        return 0;
    }


    while (length > 1)
    {
        *data++ =
            I2C_ReadByte(1);

        length--;
    }


    *data =
        I2C_ReadByte(0);


    I2C_Stop();


    return 1;
}