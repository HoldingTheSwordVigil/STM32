#include "Vofa.h"

/**
 * @brief  浮点数与整形的相互转换联合体
 * @note   
 * @retval 
 */
typedef union
{
    float fy;
    long fdata;
}FloatLongType;

/**
 * @brief  浮点型转Char型
 * @note   
 * @param  data: 
 * @param  byte: 
 * @retval None
 */
void float_to_char(float data,uint8_t* byte)
{
    FloatLongType fl;
    fl.fy = data;

    byte[0] = (char)(fl.fdata);
    byte[1] = (char)(fl.fdata >> 8);
    byte[2] = (char)(fl.fdata >> 16);
    byte[3] = (char)(fl.fdata >> 24);
}

/**
 * @brief  发送VOFA的JustFloat型数据
 * @note   
 * @param  Arry: 
 * @param  length: 
 * @retval None
 */
void Vofa_justFloat(float* Arry,uint8_t length)
{
    uint8_t fdata[4];
    uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7f};

    for (uint8_t i = 0; i < length; i++)
    {
        float_to_char(Arry[i],fdata);
        Serial_Vofa_SendArray(fdata,4);
    }
    Serial_Vofa_SendArray(tail,4);
}

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

