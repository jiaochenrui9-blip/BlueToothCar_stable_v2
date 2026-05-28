//
// Created by game on 2026/5/20.
//
#include "Encoder.h"
#include "main.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

void Encoder_Init(void)
{
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

    __HAL_TIM_SET_COUNTER(&htim2, 0);
    __HAL_TIM_SET_COUNTER(&htim3, 0);
}

int16_t Encoder_Get(uint8_t n)
{
    int16_t temp = 0;

    if (n == 1)
    {
        temp = (int16_t)__HAL_TIM_GET_COUNTER(&htim2);
        __HAL_TIM_SET_COUNTER(&htim2, 0);
        return temp;
    }

    if (n == 2)
    {
        temp = (int16_t)__HAL_TIM_GET_COUNTER(&htim3);
        __HAL_TIM_SET_COUNTER(&htim3, 0);
        return temp;
    }

    return 0;
}

float Encoder_GetSpeedRPS(uint8_t n)
{
    return (float)Encoder_Get(n) / ENCODER_COUNTS_PER_REV / ENCODER_SAMPLE_PERIOD_SEC;
}
