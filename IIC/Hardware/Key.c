#include "Key.h"

uint8_t Key_Flag = 0;

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Key1 | Key2 | Enter | Back;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void KeyScan(void)
{
    Delay_ms(20);
    if(GPIO_ReadInputDataBit(GPIOB,Key1) == 0){Key_Flag = 1;}
    if(GPIO_ReadInputDataBit(GPIOB,Key2) == 0){Key_Flag = 2;}
    if(GPIO_ReadInputDataBit(GPIOB,Enter) == 0){Key_Flag = 3;}
    if(GPIO_ReadInputDataBit(GPIOB,Back) == 0){Key_Flag = 4;}
}
