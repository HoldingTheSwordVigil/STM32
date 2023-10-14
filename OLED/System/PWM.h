#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

void PWM_Init(void);
uint16_t Moto_1_Speed(uint16_t Speed);
uint16_t Moto_2_Speed(uint16_t Speed);

#endif
