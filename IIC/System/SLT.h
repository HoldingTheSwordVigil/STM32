#ifndef __SLT_H
#define __SLT_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "stm32f10x.h"

typedef int8_t SLTDataType;        //定义链表数据类型
typedef struct SLTNode
{
    uint8_t Num;
    SLTDataType data;
    struct SLTNode* next;
}SLTNode;                       //定义链表节点



#endif
