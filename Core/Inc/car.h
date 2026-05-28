//
// Created by game on 2026/4/29.
//

#ifndef BLUETOOTHCAR_CAR_H
#define BLUETOOTHCAR_CAR_H
#include "main.h"
#include "PID.h"

typedef enum
{
    CAR_STATE_MANUAL,
    CAR_STATE_AUTO
} CarState_t;
typedef enum
{
    MANUAL_STATE_FORWARD,
    MANUAL_STATE_BACKWARD,
    MANUAL_STATE_LEFT,
    MANUAL_STATE_RIGHT,
    MANUAL_STATE_STOP
}ManualState_t;
typedef enum
{
    AUTO_STATE_FORWARD,
    AUTO_STATE_BACKWARD,
    AUTO_STATE_TURN,
    AUTO_STATE_STOP
}AUTOState_t;
extern uint8_t mode;
extern uint8_t Speed;
extern PID_TypeDef Left_Speed_PID;
extern PID_TypeDef Right_Speed_PID;
extern float TargetSpeed;
extern uint32_t autotimer;
extern CarState_t CarState;
extern ManualState_t ManualState;
extern AUTOState_t AutoState;
void CarForwards(void);
void CarBackwards(void);
void CarStop(void);
void CarLeft(void);
void CarRight(void);
void Car_Control(char cmd);
void Car_Manual_Control(void);
void Car_Auto_Control(void);
void Car_Process(void);
#endif //BLUETOOTHCAR_CAR_H
