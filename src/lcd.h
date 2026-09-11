#ifndef LCD_H
#define LCD_H

#include <stdint.h>

void LCD_Init(void);

void LCD_Clear(void);

void LCD_SetCursor(uint8_t row, uint8_t column);

void LCD_WriteChar(char value);

void LCD_WriteCommand(uint8_t command);

void LCD_Print(const char *text);

void LCD_PrintUInt(uint16_t value);

void LCD_PrintTemp10(uint16_t temperature10);

void LCD_PrintBattery(uint8_t percent);

void LCD_PrintDACVoltage(uint16_t code);

#endif