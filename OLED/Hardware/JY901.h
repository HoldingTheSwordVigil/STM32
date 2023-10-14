#ifndef __JY901_H
#define __JY901_H

#include "MY_IIC.h"
#include "JY901_Reg.h"

void JY901_GetData(uint8_t *Data);
void JY901_DisposeData(uint8_t *DataBuff, float *JY901_Data);

#endif
