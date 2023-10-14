#include "PWM.h"

#define PWM_TIMx    RCC_APB1Periph_TIM3         //PWM输出定时器
#define PWM_GPIO1   RCC_APB2Periph_GPIOA        //PWM输出口
#define PWM_GPIO2   RCC_APB2Periph_GPIOB

#define PWM_OUT_TIM     TIM3
#define PWM_OUT_IO1     GPIOA
#define PWM_OUT_IO2     GPIOB

/*
* PWM 频率 = CK_PSC/(PSC + 1）/（ARR + 1）
* PWM 占空比 = CCR/(ARR + 1)
* PWM 分辨率 = 1/(ARR + 1)
*/

/**
 * @brief  定时器PWM输出初始化
 * @note   
 * @retval None
 */
void PWM_Init(void)
{
    /**********   使能外设时钟   ************/
    RCC_APB1PeriphClockCmd(PWM_TIMx, ENABLE);
    RCC_APB2PeriphClockCmd(PWM_GPIO1, ENABLE);
    RCC_APB2PeriphClockCmd(PWM_GPIO2, ENABLE);

    /***********   GPIO配置   *************/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PWM_OUT_IO1,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(PWM_OUT_IO2,&GPIO_InitStructure);

    TIM_InternalClockConfig(PWM_OUT_TIM);      //配置TIMx内部时钟

    /***********   定时器配置   ************/
    //      频率 = 72MHz / 36 / 100 = 20KHz
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟划分
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数模式
    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;                //ARR   自动重装值
    TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1;               //PSC   预分频器
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //CCR   捕获 / 比较 寄存器
    TIM_TimeBaseInit(PWM_OUT_TIM,&TIM_TimeBaseInitStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);                         //初始化结构体
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //指定TIMx模式  为PWM1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //指定输出极性
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //使能TIM输出比较
    TIM_OCInitStructure.TIM_Pulse = 0;                              //CCR   比较寄存器

    TIM_OC1Init(PWM_OUT_TIM,&TIM_OCInitStructure);     //CH1初始化
    TIM_OC2Init(PWM_OUT_TIM,&TIM_OCInitStructure);     //CH2初始化
    TIM_OC3Init(PWM_OUT_TIM,&TIM_OCInitStructure);     //CH3初始化
    TIM_OC4Init(PWM_OUT_TIM,&TIM_OCInitStructure);     //CH4初始化

    TIM_Cmd(PWM_OUT_TIM,ENABLE);       //启动TIMx时钟
}

/**
 * @brief  设置CCR1
 * @note   
 * @param  ComPare: CCR1的值，即PWM占空比 = (Compare2 / 20,000) * 100%
 * @retval None
 */
void PWM_SetComPare1(uint16_t ComPare)
{
    TIM_SetCompare1(PWM_OUT_TIM,ComPare);
}

/**
 * @brief  设置CCR2
 * @note   
 * @param  ComPare: CCR2的值，即PWM占空比 = (Compare2 / 20,000) * 100%
 * @retval None
 */
void PWM_SetComPare2(uint16_t ComPare)
{
    TIM_SetCompare2(PWM_OUT_TIM,ComPare);
}

/**
 * @brief  设置CCR3
 * @note   
 * @param  ComPare: CCR3的值，即PWM占空比 = (Compare2 / 20,000) * 100%
 * @retval None
 */
void PWM_SetComPare3(uint16_t ComPare)
{
    TIM_SetCompare3(PWM_OUT_TIM,ComPare);
}

/**
 * @brief  设置CCR4
 * @note   
 * @param  ComPare: CCR4的值，即PWM占空比 = (Compare2 / 20,000) * 100%
 * @retval None
 */
void PWM_SetComPare4(uint16_t ComPare)
{
    TIM_SetCompare4(PWM_OUT_TIM,ComPare);
}
