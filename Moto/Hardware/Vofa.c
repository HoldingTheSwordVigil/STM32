#include "Vofa.h"

uint8_t Vofa_SendDataArryFlag[10] = {0};
float *Vofa_SendDataArry[10] = {0};

uint8_t RxBuff_Vofa[10] = {0};

PID_Handle PID_1 = {0,0,0,1,0,80,{0},100,0};
PID_Handle PID_2 = {0,0,0,1,0,20,{0},100,0};

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
 * @brief  VOFA通道初始化函数
 * @note   
 * @param  *Vofa_InitStructure: 
 * @retval None
 */
void VofaChannel_Init(Vofa_InitTypeDef *Vofa_InitStructure)
{
    Vofa_SendDataArryFlag[Vofa_InitStructure->Vofa_ChannelNum] = 1;
    Vofa_SendDataArry[Vofa_InitStructure->Vofa_ChannelNum] = Vofa_InitStructure->Vofa_ChannelVale;
}

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

float char_to_float(uint8_t *byte)
{
    uint8_t data_Start_Num = 0;     // 记录数据位开始的地方
    uint8_t data_Stop_Num = 0;     // 记录数据位结束的地方
    uint8_t data_Num = 0;           // 记录数据位数
    int8_t minus_Flag = 1;         // 判断是不是负数
    float Data_Return = 0;          // 解析得到的数据

    for (uint8_t i = 0; i < 10; i++)
    {
        if(byte[i] == '=')
        {
            data_Start_Num = i+1;
        }
        else if(byte[i] == '!')
        {
            data_Stop_Num = i;
            break;
        }
    }

    if(byte[0] == '-')
    {
        minus_Flag = -1;
        data_Start_Num = 1;
    }
    data_Num = data_Stop_Num - data_Start_Num;

    if(data_Num == 4)
    {
        Data_Return = ( minus_Flag*( (byte[data_Start_Num]-48) + (byte[data_Start_Num+2]-48)*0.1f + (byte[data_Start_Num+3]-48)*0.01f ) );
    }
    else if(data_Num == 5)
    {
        Data_Return = ( minus_Flag*( (byte[data_Start_Num]-48)*10 + (byte[data_Start_Num+1]-48)*1 + (byte[data_Start_Num+3]-48)*0.1f + (byte[data_Start_Num+4]-48)*0.01f ) );
    }

    return Data_Return;
}

/**
 * @brief  按 JustFloat 协议向 VOFA 发送数据
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

/**
 * @brief  按通道数量向 VOFA 发送数据
 * @note   
 * @param  ChannelAmount: 发送的通道数量
 * @retval None
 */
void Serial_Send_Vofa(uint8_t ChannelAmount)
{
    uint8_t fdata[4];
    uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7f};

    for (uint8_t i = 0; i < ChannelAmount; i++)
    {
        float_to_char(*Vofa_SendDataArry[i],fdata);
        Serial_Vofa_SendArray(fdata,4);
    }
    Serial_Vofa_SendArray(tail,4);
}

/**
 * @brief  将接收到的VOFA指令处理
 * @note   
 * @param  *Data_Arry: 
 * @retval None
 */
void Analysis_Assignment(uint8_t *Data_Arry)
{
    float Data = char_to_float(Data_Arry);

    if(Data_Arry[0] == 'P')
    {
        if(Data_Arry[1] == '1')
        {
            PID_1.P = Data;
        }
        else if(Data_Arry[1] == '2')
        {
            PID_2.P = Data;
        }
    }
    else if(Data_Arry[0] == 'I')
    {
        if(Data_Arry[1] == '1')
        {
            PID_1.I = Data;
        }
        else if(Data_Arry[1] == '2')
        {
            PID_2.I = Data;
        }
    }
    else if(Data_Arry[0] == 'D')
    {
        if(Data_Arry[1] == '1')
        {
            PID_1.D = Data;
        }
        else if(Data_Arry[1] == '2')
        {
            PID_2.D = Data;
        }
    }
}

void USART1_IRQHandler(void)
{
    static uint8_t RxState = 0;
    static uint8_t RXData = 0;
    static uint8_t RxCount = 0;

	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
        RXData = USART_ReceiveData(USART1);

        if(RxState == 0)
        {
            if(RXData == '@')
            {
                RxState = 1;
            }
        }
        else if(RxState == 1)
        {
            RxBuff_Vofa[RxCount] = RXData;
            RxCount++;
            if(RXData == '!')
            {
                Analysis_Assignment(RxBuff_Vofa);
                for (uint8_t i = 0; i < 10; i++)
                {
                    RxBuff_Vofa[i] = 0;
                }
                RxState = 0;
                RxCount = 0;
            }
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

