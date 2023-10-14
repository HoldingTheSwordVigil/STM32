#ifndef __VOFA_H
#define __VOFA_H

#include "VOFA_Serial.h"
#include "PID.h"

extern PID_Handle PID_1;
extern PID_Handle PID_2;

//VOFA数据通道
typedef enum
{
    Vofa_Channel_1 = 0,
    Vofa_Channel_2,
    Vofa_Channel_3,
    Vofa_Channel_4,
    Vofa_Channel_5,
    Vofa_Channel_6,
    Vofa_Channel_7,
    Vofa_Channel_8,
    Vofa_Channel_9,
    Vofa_Channel_10,
}Vofa_ChannelNumTypeDef;

/**
 * @brief  VOFA初始化配置结构体
 * @note   
 * @retval None
 */
typedef struct
{
    Vofa_ChannelNumTypeDef      Vofa_ChannelNum;            /*指定VOFA发送的通道*/
    float                       *Vofa_ChannelVale;          /*指定通道发送的值的地址*/
}Vofa_InitTypeDef;

typedef struct
{
    float *Vofa_P;
    float *Vofa_I;
    float *Vofa_D;
}Vofa_PIDInitTypeDef;

void VofaChannel_Init(Vofa_InitTypeDef *Vofa_InitStructure);
void float_to_char(float data,uint8_t* byte);
void Vofa_justFloat(float* Arry,uint8_t length);
void Serial_Send_Vofa(uint8_t ChannelAmount);

#endif
