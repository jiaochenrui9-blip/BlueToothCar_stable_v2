//
// Created by game on 2026/4/29.
//
#include "car.h"
#include "motor.h"
#include "BlueTooth.h"
#include "PID.h"
#include "ultrasonic.h"
extern uint16_t distance;
uint32_t autotimer = 0;

CarState_t CarState = CAR_STATE_MANUAL;
ManualState_t ManualState = MANUAL_STATE_STOP;
AUTOState_t AutoState = AUTO_STATE_FORWARD;
uint8_t mode;
float TargetSpeed = 1.0f;
void CarForwards(void)
{
    PID_SetTarget(&Left_Speed_PID,TargetSpeed);
    PID_SetTarget(&Right_Speed_PID,TargetSpeed);
    LeftMotorForwards();
    RightMotorForwards();
}
void CarBackwards(void)
{
    PID_SetTarget(&Left_Speed_PID,TargetSpeed);
    PID_SetTarget(&Right_Speed_PID,TargetSpeed);
    LeftMotorBackwards();
    RightMotorBackwards();

}
void CarLeft(void)
{
    PID_SetTarget(&Left_Speed_PID,0);
    PID_SetTarget(&Right_Speed_PID,TargetSpeed);
    LeftMotorStop();
    RightMotorForwards();
}
void CarRight(void)
{
    PID_SetTarget(&Left_Speed_PID,TargetSpeed);
    PID_SetTarget(&Right_Speed_PID,0);
    RightMotorStop();
    LeftMotorForwards();
}
void CarStop(void)
{
    PID_SetTarget(&Left_Speed_PID,0);
    PID_SetTarget(&Right_Speed_PID,0);
    LeftMotorStop();
    RightMotorStop();
}
void Car_Manual_Control(void)
{
    switch (ManualState)
    {
        case MANUAL_STATE_STOP:
        CarStop();
        break;
        case MANUAL_STATE_BACKWARD:
        CarBackwards();
        break;
        case MANUAL_STATE_FORWARD:
        CarForwards();
        break;
        case MANUAL_STATE_LEFT:
        CarLeft();
        break;
        case MANUAL_STATE_RIGHT:
        CarRight();
        break;
    }
}
void Car_Auto_Control(void)
{
    switch (AutoState)
    {
        case AUTO_STATE_FORWARD:
        CarForwards();
        if (distance <20 )
        {
            CarStop();
            AutoState=AUTO_STATE_STOP;
            autotimer=HAL_GetTick();
        }
        break;
        case AUTO_STATE_STOP:
        if (HAL_GetTick()-autotimer>=300)
        {
            CarBackwards();
            AutoState=AUTO_STATE_BACKWARD;
            autotimer=HAL_GetTick();
        }
        break;
        case AUTO_STATE_BACKWARD:
        if (HAL_GetTick()-autotimer>=300)
        {
            CarLeft();
            AutoState=AUTO_STATE_TURN;
            autotimer=HAL_GetTick();
        }
        break;
        case AUTO_STATE_TURN:
        if (HAL_GetTick()-autotimer>=300)
        {
            AutoState = AUTO_STATE_FORWARD;
        }
    }
}
void Car_Process(void)
{
    switch (CarState)
    {
        case CAR_STATE_MANUAL:
        Car_Manual_Control();
        break;
        case CAR_STATE_AUTO:
        Car_Auto_Control();
        break;
    }
}
