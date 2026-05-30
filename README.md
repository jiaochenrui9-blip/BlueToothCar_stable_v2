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

## Status

Stable learning version with state machine and speed PID logic saved.
