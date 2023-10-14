#ifndef __MY_ICC_H
#define __MY_IIC_H

#include "Sys.h"
#include "Delay.h"

#define SDA_IN()    {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=0x00008000;}
#define SDA_OUT()   {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=0x00003000;}

#define IIC_SCL     PBout(10) //SCL
#define IIC_SDA     PBout(11) //SDA
#define READ_SDA    PBin(11)

void MY_IIC_Init(void);
void MY_IIC_Start(void);
void MY_IIC_Stop(void);
uint8_t MY_IIC_Wait_ACK(void);
void MY_IIC_Ack(void);
void MY_IIC_NAck(void);
void MY_IIC_Send_Byte(uint8_t Byte);
uint8_t MY_IIC_Read_Byte(void);
uint8_t MY_IIC_Write_Data(uint8_t Address,uint8_t RegAddress, uint8_t *Data, uint8_t Length);
uint8_t MY_IIC_Read_Data(uint8_t Address,uint8_t RegAddress, uint8_t *Data, uint8_t Length);

#endif
