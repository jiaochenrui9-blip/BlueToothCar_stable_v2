# BlueToothCar Stable V2

STM32 smart car project with Bluetooth control, obstacle avoidance, OLED status display, encoder speed feedback, and PID speed control.

## Features

- Bluetooth command control
- Manual and automatic car state switching
- Non-blocking state machine based on HAL ticks
- OLED debug display
- Ultrasonic distance check for obstacle avoidance
- Encoder speed measurement
- PID speed control for motor PWM output

## Hardware

- STM32F103 series board
- DC motors and motor driver
- Bluetooth serial module
- Ultrasonic distance module
- OLED display
- Wheel encoders

## Development Environment

- STM32CubeMX
- CLion
- STM32 HAL
- CMake

## 项目中遇到的问题

1. OLED 上电后容易卡住不动

   一开始 OLED 有时上电后会卡住，显示不刷新。后面把 OLED 直接插到面包板上，减少接触不良和杜邦线松动带来的影响，问题得到解决。

2. 串口发送调试信息一开始卡了很久

   串口调试最开始比较混乱，主要原因是发送逻辑没有封装成函数，导致代码分散、不方便排查。后面把串口发送调试信息整理成独立函数，主循环里只负责定时调用，代码结构更清楚，也更容易调试。

3. PID 调速时反转速度为负数导致输出越来越大

   电机反转时，编码器测出来的速度是负数。如果直接用目标速度和负速度做误差计算，误差会变得很大，PID 会不断增大 `out` 输出，导致电机越转越快。后面改成“PID 只控制速度大小，方向单独控制”的方法：速度取大小参与 PID，正反转方向由电机控制函数单独决定。

## Status

Stable learning version with state machine and speed PID logic saved.
