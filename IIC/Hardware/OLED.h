#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"
#include "MY_IIC.h"
#include "Key.h"
#include <stdio.h>
#include <math.h>

void OLED_Fill(uint8_t Fill_Data);
void OLED_Clear(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char, uint16_t Size, uint8_t Inverse);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String, uint16_t Size, uint8_t Inverse);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint16_t Size, uint8_t Inverse);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length, uint16_t Size, uint8_t Inverse);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint16_t Size, uint8_t Inverse);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint16_t Size, uint8_t Inverse);
void OLED_LevelRun(uint8_t LineMin, uint8_t LineMax, uint8_t dir, uint8_t Speed);
void OLED_Init(void);

void OLED_DrawPoint(uint8_t X,uint8_t Y);
void OLED_DrawLine(uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2);
void OLED_MathLine(int8_t X1, int8_t Y1,int8_t X2, int8_t Y2);
void OLED_Refresh_GRAM(void);
void Clear_GRAM(void);

typedef struct menu_table
{
    char Show_Name[10];          //菜单名称
    void (*func1)(void);            //当前菜单应执行的操作
    void (*func2)(void);            //当前菜单可执行的操作
    struct menu_table *fatherMenu;  //当前菜单的父级菜单
    struct menu_table *childMenu;   //当前菜单的子级菜单
}Menu_Table;

extern uint8_t NowIndex;
extern Menu_Table Home_Menu;
extern Menu_Table Main_Menu[10];
extern Menu_Table Set_Menu[10];
extern Menu_Table PID_Menu[10];
extern Menu_Table GAME_Menu[10];
extern Menu_Table Gyro_Menu[10];

void RefreshMenue(void);

void ShowHome(void);
void ShowMain(void);
void ShowSet(void);
void ShowPID(void);
void ShowGAME(void);
void ShowGyro(void);

#endif
