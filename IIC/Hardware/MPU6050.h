#ifndef __MPU6050_H
#define __MPU6050_H

#include "MY_IIC.h"
#include "MPU6050_Reg.h"

void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_GetData(uint8_t *Data);
void MPU6050_DisposeData(uint8_t *DataBuff, float *MPU6050_Data);

#endif
