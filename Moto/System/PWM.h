#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

void PWM_Init(void);
void PWM_SetComPare1(uint16_t ComPare);
void PWM_SetComPare2(uint16_t ComPare);
void PWM_SetComPare3(uint16_t ComPare);
void PWM_SetComPare4(uint16_t ComPare);

#endif
