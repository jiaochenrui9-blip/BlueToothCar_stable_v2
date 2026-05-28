//
// Created by game on 2026/4/29.
//
#include "main.h"
#include "motor.h"
#include "stdint.h"
void RightMotorForwards(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
}
void RightMotorBackwards(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
}
void RightMotorStop(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
}
void LeftMotorForwards(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
}
void LeftMotorBackwards(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
}
void LeftMotorStop(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
}
void MotorSetSpeed(uint8_t speed)
{
    if (speed > 100)
    {
        speed = 100;
    }
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, speed);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, speed);
}

void MotorSetLeftPWM(uint8_t pwm)
{
    if (pwm > 99)
    {
        pwm = 99;
    }

    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, pwm);
}

void MotorSetRightPWM(uint8_t pwm)
{
    if (pwm > 99)
    {
        pwm = 99;
    }

    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, pwm);
}
