#include "MY_IIC.h"

#ifdef Software_IIC

/**
 * @brief  IIC初始化函数
 * @note   
 * @retval None
 */
void MY_IIC_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    SDA_OUT();
    IIC_SDA = 1;
    IIC_SCL = 1;
}

/**
 * @brief  IIC开始函数
 * @note   
 * @retval None
 */
void MY_IIC_Start(void)
{
    SDA_OUT();

    IIC_SDA = 1;
    IIC_SCL = 1;
    Delay_us(5);

    IIC_SDA = 0;
    Delay_us(5);

    IIC_SCL = 0;
}

/**
 * @brief  IIC结束函数
 * @note   
 * @retval None
 */
void MY_IIC_Stop(void)
{
    SDA_OUT();

    IIC_SDA = 0;
    Delay_us(5);

    IIC_SCL = 1;
    IIC_SDA = 1;
    Delay_us(5);
}

/**
 * @brief  IIC等待从机应答
 * @note   
 * @retval 
 */
uint8_t MY_IIC_Wait_ACK(void)
{
    uint8_t WaitTime = 0;
    SDA_IN();

    IIC_SDA = 1;
    Delay_us(5);

    while(READ_SDA)
    {
        WaitTime++;
        if(WaitTime > 50)
        {
            MY_IIC_Stop();
            return 1;
        }
    }
    IIC_SCL = 1;
    Delay_us(5);

    IIC_SCL = 0;
    return 0;
}

/**
 * @brief  IIC发送应答位
 * @note   
 * @retval None
 */
void MY_IIC_Ack(void)
{
    SDA_OUT();

    IIC_SCL = 0;
    IIC_SDA = 0;
    Delay_us(5);

    IIC_SCL = 1;
    Delay_us(5);

    IIC_SCL = 0;
    Delay_us(5);
}
/**
 * @brief  IIC发送非应答位
 * @note   
 * @retval None
 */
void MY_IIC_NAck(void)
{
    SDA_OUT();

    IIC_SCL = 0;
    IIC_SDA = 1;
    Delay_us(5);

    IIC_SCL = 1;
    Delay_us(5);

    IIC_SCL = 0;
    Delay_us(5);
}

/**
 * @brief  IIC发送 1字节数据
 * @note   
 * @param  Byte: 
 * @retval None
 */
void MY_IIC_Send_Byte(uint8_t Byte)
{
    SDA_OUT();

    for (uint8_t i = 0; i < 8; i++)
    {
        IIC_SDA = (Byte & 0x80) >> 7;
        Byte <<= 1;
        Delay_us(2);

        IIC_SCL = 1;
        Delay_us(5);

        IIC_SCL = 0;
        Delay_us(3);
    }
    Delay_us(2);
}

/**
 * @brief  IIC接收 1字节数据
 * @note   
 * @retval 
 */
uint8_t MY_IIC_Read_Byte(void)
{
    uint8_t Data = 0x00;
    SDA_IN();

    for (uint8_t i = 0; i < 8; i++)
    {
        IIC_SCL = 1;
        if(READ_SDA == 1){Data |= (0x80 >> i);}
        Delay_us(5);

        IIC_SCL = 0;
        Delay_us(5);
    }
    return Data;
}

/**
 * @brief  IIC写入数据
 * @note   
 * @param  Address: 从机地址
 * @param  RegAddress: 寄存器地址
 * @param  *Data: 写入的数据
 * @param  Length: 数据的长度
 * @retval 
 */
uint8_t MY_IIC_Write_Data(uint8_t Address,uint8_t RegAddress, uint8_t *Data, uint8_t Length)
{
    MY_IIC_Start();
    MY_IIC_Send_Byte(Address << 1);
    if(MY_IIC_Wait_ACK() == 1){return 0;}

    MY_IIC_Send_Byte(RegAddress);
    if(MY_IIC_Wait_ACK() == 1){return 0;}

    for (uint8_t i = 0; i < Length; i++)
    {
        MY_IIC_Send_Byte(Data[i]);
        if(MY_IIC_Wait_ACK() == 1){return 0;}
    }
    MY_IIC_Stop();

    return 1;
}

/**
 * @brief  IIC读取数据
 * @note   
 * @param  Address: 从机地址
 * @param  RegAddress: 寄存器地址
 * @param  *Data: 读取的数据
 * @param  Length: 数据的长度
 * @retval 
 */
uint8_t MY_IIC_Read_Data(uint8_t Address,uint8_t RegAddress, uint8_t *Data, uint8_t Length)
{
    MY_IIC_Start();
    MY_IIC_Send_Byte(Address << 1);
    if(MY_IIC_Wait_ACK() == 1){return 0;}

    MY_IIC_Send_Byte(RegAddress);
    if(MY_IIC_Wait_ACK() == 1){return 0;}

    MY_IIC_Start();
    MY_IIC_Send_Byte((Address << 1) | 0x01);
    if(MY_IIC_Wait_ACK() == 1){return 0;}

    for (uint8_t i = 0; i < Length; i++)
    {
        if(i < (Length-1))
        {
            Data[i] = MY_IIC_Read_Byte();
            MY_IIC_Ack();
        }
        else
        {
            Data[i] = MY_IIC_Read_Byte();
            MY_IIC_NAck();
        }
    }
    MY_IIC_Stop();

    return 1;
}

#else

/**
 * @brief  IIC初始化函数
 * @note   
 * @retval None
 */
void MY_IIC_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_ClockSpeed = 200000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_OwnAddress1 = 0x05;
	I2C_Init(I2C2, &I2C_InitStructure);

    I2C_Cmd(I2C2, ENABLE);
}

uint8_t IIC_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout --;
		if (Timeout == 0)
		{
			return 1;
		}
	}

    return 0;
}

/**
 * @brief  IIC写入数据
 * @note   
 * @param  Address: 从机地址
 * @param  RegAddress: 寄存器地址
 * @param  *Data: 写入的数据
 * @param  Length: 数据的长度
 * @retval 
 */
uint8_t MY_IIC_Write_Data(uint8_t Address,uint8_t RegAddress, uint8_t *Data, uint8_t Length)
{
	I2C_GenerateSTART(I2C2, ENABLE);
	if(IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))return 0;
	
	I2C_Send7bitAddress(I2C2, Address << 1, I2C_Direction_Transmitter);
	if(IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))return 0;
	
	I2C_SendData(I2C2, RegAddress);
	if(IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))return 0;
	
    for (uint8_t i = 0; i < Length; i++)
    {
        I2C_SendData(I2C2, Data[i]);
	    if(IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))return 0;
    }
	
	I2C_GenerateSTOP(I2C2, ENABLE);

    return 1;
}

/**
 * @brief  IIC读取数据
 * @note   
 * @param  Address: 从机地址
 * @param  RegAddress: 寄存器地址
 * @param  *Data: 读取的数据
 * @param  Length: 数据的长度
 * @retval 
 */
uint8_t MY_IIC_Read_Data(uint8_t Address,uint8_t RegAddress, uint8_t *Data, uint8_t Length)
{
    I2C_GenerateSTART(I2C2, ENABLE);
	if(IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))return 0;

    I2C_Send7bitAddress(I2C2, Address << 1, I2C_Direction_Transmitter);
	if(IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))return 0;
	
	I2C_SendData(I2C2, RegAddress);
	if(IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))return 0;
	
	I2C_GenerateSTART(I2C2, ENABLE);
	if(IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))return 0;
	
	I2C_Send7bitAddress(I2C2, (Address << 1) | 0x01, I2C_Direction_Receiver);
	if(IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))return 0;

    for (uint8_t i = 0; i < Length; i++)
    {
        if(i == (Length-1))
        {
            Data[i] = I2C_ReceiveData(I2C2);
            I2C_AcknowledgeConfig(I2C2, DISABLE);
        }
        else
        {
            Data[i] = I2C_ReceiveData(I2C2);
        }
    }
    I2C_GenerateSTOP(I2C2, ENABLE);

    I2C_AcknowledgeConfig(I2C2, ENABLE);

    return 1;
}

#endif
