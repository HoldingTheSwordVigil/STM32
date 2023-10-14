#include "Key.h"

#define Key1_Pin        GPIO_Pin_12
#define Key2_Pin        GPIO_Pin_13
#define Enter_Pin       GPIO_Pin_14
#define Back_Pin        GPIO_Pin_15

#define Key1        GPIO_ReadInputDataBit(GPIOB, Key1_Pin)
#define Key2        GPIO_ReadInputDataBit(GPIOB, Key2_Pin)
#define Enter       GPIO_ReadInputDataBit(GPIOB, Enter_Pin)
#define Back        GPIO_ReadInputDataBit(GPIOB, Back_Pin)

#define Next        1
#define Lest        2
#define Entern      3
#define Rollback    4

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Key1_Pin | Key2_Pin | Enter_Pin | Back_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

uint8_t KeyScan(void)
{
    Delay_ms(10);
    if(Key1 == 0){return Next;}
    if(Key2 == 0){return Lest;}
    if(Enter == 0){return Entern;}
    if(Back == 0){return Rollback;}
    return 0;
}
