#ifndef __VOFA_Serial_H
#define __VOFA_Serial_H

#include "stm32f10x.h"                  // Device header

#define Serial_Line     USART1
#define Serial_GPIO		GPIOA
#define Serial_Txd 		GPIO_Pin_9
#define Serial_Rxd 		GPIO_Pin_10

void Serial_Vofa_Init(void);
void Serial_Vofa_SendByte(uint8_t Byte);
void Serial_Vofa_SendArray(uint8_t *Array, uint16_t Length);
void Serial_Vofa_SendString(char *String);

#endif
