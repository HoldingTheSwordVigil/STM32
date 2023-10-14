#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"                  // Device header

#include "u8g2.h"
#include "Delay.h"
#include "sys.h"
#include "Key.h"
#include "JY901.h"
#include "Vofa.h"
#include "Moto.h"

extern u8g2_t OLED_Structure;

void OLED_Init(void);

typedef struct menu_table
{
    char menu_name[10];             //菜单名称
    void (*UIfunc)(void);           //UI绘制
    void (*func1)(void);            //菜单可使用的功能
    struct menu_table *fatherMenu;  //菜单的父级菜单
    struct menu_table *childMenu;   //菜单的子级菜单
}Menu_Table;

extern Menu_Table Main_Menu[10];
extern Menu_Table Set_Menu[10];
extern Menu_Table PID_Menu[10];
extern Menu_Table GAME_Menu[10];
extern Menu_Table Gyro_Menu[12];
extern Menu_Table Moto_Menu[10];

void HomeUI(void);
void MainUI(void);
void SetUI(void);
void PIDUI(void);
void GAMEUI(void);
void GyroUI(void);
void MotoUI(void);

void PID_Data(void);
void P_set(void);
void I_set(void);
void D_set(void);
void Gyro_GetData(void);
void Get_MotoState(void);

void RefreshMenue(void);

#endif
