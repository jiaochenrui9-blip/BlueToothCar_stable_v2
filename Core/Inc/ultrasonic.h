#ifndef BLUETOOTHCAR_ULTRASONIC_H
#define BLUETOOTHCAR_ULTRASONIC_H

#include <stdint.h>

void ultrasonic_Send(void);
uint32_t ultrasonic_Get_Time(void);
uint16_t Get_distance(void);
void ultrasonic_Scan(uint16_t *left,uint16_t *front,uint16_t *right);
#endif