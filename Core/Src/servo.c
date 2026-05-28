#include "main.h"
#include "servo.h"

#define SERVO_MIN_US 500
#define SERVO_MAX_US 2500
#define SERVO_MAX_ANGLE 180

void Servo_Init(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    Servo_SetAngle(90);
}

void Servo_SetAngle(uint8_t angle)
{
    uint16_t pulse;

    if (angle > SERVO_MAX_ANGLE)
    {
        angle = SERVO_MAX_ANGLE;
    }

    pulse = SERVO_MIN_US + (uint16_t)((SERVO_MAX_US - SERVO_MIN_US) * angle / SERVO_MAX_ANGLE);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pulse);
}