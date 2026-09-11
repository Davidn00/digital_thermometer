#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>

#define BUTTON_NONE     0x00
#define BUTTON_MODE     0x01
#define BUTTON_SELECT   0x02

void Buttons_Init(void);

uint8_t Buttons_Update(void);

#endif