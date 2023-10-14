#include "JY901.h"

/**
 * @brief  获取JY901的数据
 * @note   
 * @param  *Data: 存放数据的数组
 * @retval None
 */
void JY901_GetData(uint8_t *Data)
{
    MY_IIC_Read_Data(JY901_ADDR,AX,Data,26);
}

/**
 * @brief  解算JY901的数据
 * @note   
 * @param  *DataBuff: 原始数据
 * @param  *JY901_Data: 处理后的数据
 * @retval None
 */
void JY901_DisposeData(uint8_t *DataBuff, float *JY901_Data)
{
    int16_t Data[13] = {0};

    for (uint8_t i = 0; i < 13; i++)
    {
        Data[i] = (short)(DataBuff[(i << 1)+1] << 8) | (DataBuff[i << 1]);
    }

    JY901_Data[0] = (float)Data[0] / 32768.0f * 16.0f;
    JY901_Data[1] = (float)Data[1] / 32768.0f * 16.0f;
    JY901_Data[2] = (float)Data[2] / 32768.0f * 16.0f;

    JY901_Data[3] = (float)Data[3] / 32768.0f * 2000.0f;
    JY901_Data[4] = (float)Data[4] / 32768.0f * 2000.0f;
    JY901_Data[5] = (float)Data[5] / 32768.0f * 2000.0f;

    JY901_Data[6] = (float)Data[9] / 32768.0f * 180.0f;
    JY901_Data[7] = (float)Data[10] / 32768.0f * 180.0f;
    JY901_Data[8] = (float)Data[11] / 32768.0f * 180.0f;

    JY901_Data[9] = (float)Data[6];
    JY901_Data[10] = (float)Data[7];
    JY901_Data[11] = (float)Data[8];

    JY901_Data[12] = (float)Data[12] / 100.0f;
}
