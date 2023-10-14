#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#include "Delay.h"

#define Key1        GPIO_Pin_12
#define Key2        GPIO_Pin_13
#define Enter       GPIO_Pin_14
#define Back        GPIO_Pin_15

extern uint8_t Key_Flag;

void Key_Init(void);
void KeyScan(void);

#endif
