#include "Gyro.h"

#ifdef MPU6050_Gyro

/**
 * @brief  MPU6050初始化函数
 * @note   
 * @retval None
 */
void MPU6050_Init(void)
{
    uint8_t a[] = {0x01,0x00},b[] = {0x09,0x06,0x18,0x18};

    MY_IIC_Write_Data(MPU6050Address,MPU6050_PWR_MGMT_1,a,2);
    MY_IIC_Write_Data(MPU6050Address,MPU6050_SMPLRT_DIV,b,4);
}

/**
 * @brief  获取MPU6050的地址
 * @note   
 * @retval 
 */
uint8_t MPU6050_GetID(void)
{
    uint8_t ID = 0;

    MY_IIC_Read_Data(MPU6050Address,MPU6050_WHO_AM_I,&ID,1);

    return ID;
}

/**
 * @brief  获取MPU6050的数据
 * @note   
 * @param  *Data: 存储数据的数组
 * @retval None
 */
void MPU6050_GetData(uint8_t *Data)
{
    MY_IIC_Read_Data(MPU6050Address,MPU6050_ACCEL_XOUT_H,Data,14);
}

/**
 * @brief  对获取的数据进行处理
 * @note   
 * @param  *DataBuff: 获取到的原始数据 数组
 * @param  *MPU6050_Data: 处理好的数据 数组
 * @retval None
 */
void MPU6050_DisposeData(uint8_t *DataBuff, float *MPU6050_Data)
{
    int16_t Data[7] = {0};

    for (uint8_t i = 0; i < 7; i++)
    {
        Data[i] = (int16_t)(DataBuff[i << 1] << 8) | (DataBuff[(i << 1) + 1]);
    }
    /*换算
    * 加速度    16.0
    * 角速度    2000
    * 角度      180
    * */
    MPU6050_Data[0] = (float)Data[0] / 32768.0f * 16.0f;
    MPU6050_Data[1] = (float)Data[1] / 32768.0f * 16.0f;
    MPU6050_Data[2] = (float)Data[2] / 32768.0f * 16.0f;

    MPU6050_Data[3] = 36.53 + ((float)Data[3] / 340.0f);

    MPU6050_Data[4] = (float)Data[4] / 32768.0f * 2000.0f;
    MPU6050_Data[5] = (float)Data[5] / 32768.0f * 2000.0f;
    MPU6050_Data[6] = (float)Data[6] / 32768.0f * 2000.0f;
}


#elif defined(JY901_Gyro)

/**
 * @brief  获取JY901的数据
 * @note   
 * @param  *Data: 存放数据的数组
 * @retval None
 */
void JY901_GetData(uint8_t *Data)
{
    MY_IIC_Read_Data(JY901Address,AX,Data,26);
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


#endif
