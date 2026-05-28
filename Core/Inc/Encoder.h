//
// Created by game on 2026/5/20.
//

#ifndef BLUETOOTHCAR_ENCODER_H
#define BLUETOOTHCAR_ENCODER_H

#include <stdint.h>

#define ENCODER_COUNTS_PER_REV      1560.0f
#define ENCODER_SAMPLE_PERIOD_SEC   0.02f

void Encoder_Init(void);
int16_t Encoder_Get(uint8_t n);
float Encoder_GetSpeedRPS(uint8_t n);

#endif //BLUETOOTHCAR_ENCODER_H
