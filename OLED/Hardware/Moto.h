#ifndef __MOTO_H
#define __MOTO_H

#include "PWM.h"

void Moto_Init(void);
uint16_t L_MotoState(uint16_t Speed, uint8_t Dir);
uint16_t R_MotoState(uint16_t Speed, uint8_t Dir);

#endif
