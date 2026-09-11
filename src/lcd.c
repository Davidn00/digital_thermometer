#include <xc.h>
#include <stdint.h>

#include "config.h"
#include "lcd.h"
#include "pcf8574.h"

#define LCD_RS      0x01
#define LCD_RW      0x02
#define LCD_E       0x04
#define LCD_BL      0x08


static void LCD_PulseEnable(uint8_t value)
{
    PCF8574_Write(value | LCD_E);

    __delay_us(1);

    PCF8574_Write(value & ~LCD_E);

    __delay_us(50);
}


static void LCD_SendNibble(
    uint8_t nibble,
    uint8_t rs
)
{
    uint8_t value;


    value = LCD_BL;

    if (rs)
    {
        value |= LCD_RS;
    }


    value |= (nibble & 0x0F) << 4;


    PCF8574_Write(value);

    LCD_PulseEnable(value);
}


static void LCD_SendByte(
    uint8_t value,
    uint8_t rs
)
{
    LCD_SendNibble(
        value >> 4,
        rs
    );


    LCD_SendNibble(
        value & 0x0F,
        rs
    );
}


void LCD_Init(void)
{
    PCF8574_Init();

    __delay_ms(50);


    LCD_SendNibble(0x03, 0);
    __delay_ms(5);

    LCD_SendNibble(0x03, 0);
    __delay_us(150);

    LCD_SendNibble(0x03, 0);

    LCD_SendNibble(0x02, 0);


    LCD_WriteCommand(0x28); /* 4-bit, 2-line mode */
    LCD_WriteCommand(0x08); /* Display OFF */
    LCD_WriteCommand(0x01); /* Clear */
    __delay_ms(2);

    LCD_WriteCommand(0x06); /* Entry mode */
    LCD_WriteCommand(0x0C); /* Display ON */
}


void LCD_Clear(void)
{
    LCD_WriteCommand(0x01);

    __delay_ms(2);
}


void LCD_SetCursor(
    uint8_t row,
    uint8_t column
)
{
    static const uint8_t row_address[] =
    {
        0x00,
        0x40,
        0x14,
        0x54
    };


    if (row > 3)
    {
        row = 0;
    }


    if (column > 19)
    {
        column = 19;
    }


    LCD_WriteCommand(
        0x80 |
        (row_address[row] + column)
    );
}


void LCD_WriteCommand(uint8_t command)
{
    LCD_SendByte(command, 0);

    if (command == 0x01)
    {
        __delay_ms(2);
    }
}


void LCD_WriteChar(char value)
{
    LCD_SendByte(
        (uint8_t)value,
        1
    );
}


void LCD_Print(const char *text)
{
    while (*text)
    {
        LCD_WriteChar(*text);

        text++;
    }
}


void LCD_PrintUInt(uint16_t value)
{
    char buffer[6];

    uint8_t i = 0;


    if (value == 0)
    {
        LCD_WriteChar('0');

        return;
    }


    while (value > 0)
    {
        buffer[i++] =
            '0' + (value % 10U);

        value /= 10U;
    }


    while (i > 0)
    {
        LCD_WriteChar(
            buffer[--i]
        );
    }
}


void LCD_PrintTemp10(uint16_t temperature10)
{
    uint16_t integer_part;

    uint8_t decimal_part;


    integer_part =
        temperature10 / 10U;

    decimal_part =
        temperature10 % 10U;


    LCD_PrintUInt(integer_part);

    LCD_WriteChar('.');

    LCD_WriteChar(
        '0' + decimal_part
    );

    LCD_WriteChar(' ');

    LCD_WriteChar('C');
}


void LCD_PrintBattery(uint8_t percent)
{
    LCD_PrintUInt(percent);

    LCD_WriteChar('%');
}


void LCD_PrintDACVoltage(uint16_t code)
{
    uint32_t millivolts;

    uint16_t volts;

    uint16_t decimal;


    millivolts =
        ((uint32_t)code * 5000UL) /
        4095UL;


    volts =
        (uint16_t)(millivolts / 1000UL);

    decimal =
        (uint16_t)(millivolts % 1000UL);


    LCD_PrintUInt(volts);

    LCD_WriteChar('.');

    LCD_WriteChar(
        '0' + ((decimal / 100U) % 10U)
    );

    LCD_WriteChar(
        '0' + ((decimal / 10U) % 10U)
    );

    LCD_WriteChar(
        '0' + (decimal % 10U)
    );

    LCD_WriteChar('V');
}