#include "PWM.h"

/*
* PWM 频率 = CK_PSC/(PSC + 1）/（ARR + 1）
* PWM 占空比 = CCR/(ARR + 1)
* PWM 分辨率 = 1/(ARR + 1)
*/
void PWM_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    TIM_InternalClockConfig(TIM3);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;             //ARR    重载值
    TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1;           //PSC   预分频器
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);  //20KHz

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;  //CCR   比较寄存器

    TIM_OC1Init(TIM3,&TIM_OCInitStructure);
    TIM_OC2Init(TIM3,&TIM_OCInitStructure);

    TIM_Cmd(TIM3,ENABLE);
}

uint16_t Moto_1_Speed(uint16_t Speed)
{
    TIM_SetCompare1(TIM3, Speed);
    return Speed;
}

uint16_t Moto_2_Speed(uint16_t Speed)
{
    TIM_SetCompare2(TIM3, Speed);
    return Speed;
}
