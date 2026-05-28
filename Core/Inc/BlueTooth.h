//
// Created by game on 2026/4/29.
//

#ifndef BLUETOOTHCAR_BLUETOOTH_H
#define BLUETOOTHCAR_BLUETOOTH_H
#include <stdint.h>
extern uint8_t Receive_Data;
void BlueTooth_Init(void);
void Flag_Clear(void);
uint8_t BlueTooth_GetData(void);
uint8_t BlueTooth_GetFlag(void);
void Car_BlueTooth_Cmd(char cmd);
#endif //BLUETOOTHCAR_BLUETOOTH_H
