//
// Created by game on 2026/4/29.
//

#ifndef BLUETOOTHCAR_MOTOR_H
#define BLUETOOTHCAR_MOTOR_H

#include "stdint.h"
void RightMotorForwards(void);
void RightMotorBackwards(void);
void LeftMotorForwards(void);
void LeftMotorBackwards(void);
void RightMotorStop(void);
void LeftMotorStop(void);
void MotorSetSpeed(uint8_t speed);
void MotorSetLeftPWM(uint8_t pwm);
void MotorSetRightPWM(uint8_t pwm);
extern uint8_t Speed;


#endif //BLUETOOTHCAR_MOTOR_H
