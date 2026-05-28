//
// Created by game on 2026/5/15.
//
#include "main.h"

#include "PID.h"

#include "motor.h"

#define DEADZONE 10
#define POSITION_MAX_SPEED 40

void PID_SetTarget(PID_TypeDef *pid,float target)
{
    pid->target=target;
}
float PID_Calculate(PID_TypeDef *pid)
{
    pid->error1=pid->error0;
    pid->error0 = pid->target - pid->actual;
    pid->integral += pid->error0;
    if (pid->integral > pid->integralMax)
    {pid->integral = pid->integralMax;}

    else if (pid->integral < pid->integralMin)
    {pid->integral = pid->integralMin;}

    pid->out = pid->Kp * pid->error0
             + pid->Ki * pid->integral
             + pid->Kd * (pid->error0 - pid->error1);
    if (pid->out > pid->outMax){pid->out = pid->outMax;}
    else if (pid->out < pid->outMin){pid->out = pid->outMin;}
    return pid->out;
}
void PID_Reset(PID_TypeDef *pid)
{
    pid->actual = 0.0f;

    pid->error0 = 0.0f;
    pid->error1 = 0.0f;

    pid->integral = 0.0f;

    pid->out = 0.0f;
}
