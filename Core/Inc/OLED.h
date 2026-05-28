#ifndef BLUETOOTHCAR_OLED_H
#define BLUETOOTHCAR_OLED_H

#include "main.h"
#include <stdint.h>

void OLED_Init(void);
void OLED_Clear(void);
void OLED_Update(void);
void OLED_ShowString(uint8_t x, uint8_t y, const char *str);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num);
void OLED_ShowCarInfo(uint16_t distance);
void OLED_ShowCarState(uint16_t distance, uint8_t cmd);

#endif
