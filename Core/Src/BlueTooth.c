//
// Created by game on 2026/4/29.
//
#include "main.h"
#include "BlueTooth.h"
#include "car.h"
uint8_t Receive_Data = 0;
static volatile uint8_t Receive_Flag = 0;
void BlueTooth_Init(void)
{
    HAL_UART_Receive_IT(&huart1, &Receive_Data, 1);
}
uint8_t BlueTooth_GetData(void)
{
    return Receive_Data;
}
uint8_t BlueTooth_GetFlag(void)
{
    return Receive_Flag;
}
void Flag_Clear(void)
{
    Receive_Flag = 0;
}
void Car_BlueTooth_Cmd(char cmd)
{
    switch (cmd)
    {
        case 'M':
        CarState = CAR_STATE_MANUAL;
        ManualState=MANUAL_STATE_STOP;
        break;
        case 'S':
        CarState = CAR_STATE_MANUAL;
        ManualState=MANUAL_STATE_STOP;
        break;
        case 'L':
        CarState = CAR_STATE_MANUAL;
        ManualState=MANUAL_STATE_LEFT;
        break;
        case 'R':
        CarState = CAR_STATE_MANUAL;
        ManualState=MANUAL_STATE_RIGHT;
        break;
        case 'B':
        CarState = CAR_STATE_MANUAL;
        ManualState=MANUAL_STATE_BACKWARD;
        break;
        case 'F':
        CarState = CAR_STATE_MANUAL;
        ManualState=MANUAL_STATE_FORWARD;
        break;
        case 'A':
        CarState = CAR_STATE_AUTO;
        AutoState=AUTO_STATE_FORWARD;
        autotimer=HAL_GetTick();
        break;

    }
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        Receive_Flag=1;
        HAL_UART_Receive_IT(&huart1, &Receive_Data, 1);
    }
}
