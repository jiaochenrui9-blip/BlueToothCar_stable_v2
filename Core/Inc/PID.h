#ifndef __PID_H
#define __PID_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/* PID结构体 */
typedef struct
{
    float Kp;
    float Ki;
    float Kd;
    float Kpo;//定位置比例项

    float target;      // 目标值
    float actual;      // 实际值

    float error0;      // 当前误差
    float error1;      // 上一次误差

    float integral;    // 积分项

    float out;         // PID输出

    float outMax;      // 输出最大值
    float outMin;      // 输出最小值

    float integralMax; // 积分最大值
    float integralMin; // 积分最小值



} PID_TypeDef;


/* 设置目标值 */
void PID_SetTarget(PID_TypeDef *pid, float target);

/* PID计算 */
float PID_Calculate(PID_TypeDef *pid);

/* PID清零 */
void PID_Reset(PID_TypeDef *pid);
/*定位置*/
void SetPosition(PID_TypeDef *pid, float targetpos);
#ifdef __cplusplus
}
#endif

#endif