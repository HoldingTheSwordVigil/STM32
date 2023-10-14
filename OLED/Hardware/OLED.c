#include "OLED.h"
#include <stdio.h>

uint8_t Key_State = 0;

/**
 * @brief  重定向 printf 在OLED上打印
 * @note   
 * @param  x: 左下角的x坐标
 * @param  y: 左下角的y坐标
 * @param  *font: 指定字体
 *          常用英文字体：u8g2_font_unifont_t_symbols
 *          常用中文字体：u8g2_font_wqy16_t_chinese1
 * @param  Tou: 设置字体透明
 *              0：透明
 *              1：不透明
 * @param  *pFormat: 
 * @retval 
 */
signed int OLED_printf(uint8_t x, uint8_t y, const uint8_t *font, uint8_t Tou, const char *pFormat, ...){

    char pStr[25] = {'\0'}; 
    va_list ap;
    signed int result;

    // Forward call to vprintf
    va_start(ap, pFormat);
    result = vsprintf((char *)pStr, pFormat, ap);
    va_end(ap);

    u8g2_SetFont(&OLED_Structure, font); /*字库选择*/
    u8g2_SetDrawColor(&OLED_Structure, Tou);
    u8g2_DrawStr(&OLED_Structure,x,y,(const char *)pStr);
    u8g2_SetDrawColor(&OLED_Structure, 1);

    return result;
}

/***********************************************************************************************************************/
//  U8g2移植
/***********************************************************************************************************************/
#define SCL_Pin 		GPIO_Pin_8
#define SDA_Pin 		GPIO_Pin_9
#define IIC_GPIO_Port 	GPIOB
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );

	GPIO_InitStructure.GPIO_Pin = SCL_Pin|SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_GPIO_Port, &GPIO_InitStructure);
}

uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
        __NOP();
        break;
    case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
        for (uint16_t n = 0; n < 320; n++)
        {
            __NOP();
        }
        break;
    case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
        Delay_ms(1);
        break;
    case U8X8_MSG_DELAY_I2C: // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
        Delay_us(5);
        break;                    // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int=0: Output low at I2C clock pin
		if(arg_int == 1)
		{
			GPIO_SetBits(IIC_GPIO_Port, SCL_Pin);
		}
		else if(arg_int == 0)
		{
			GPIO_ResetBits(IIC_GPIO_Port, SCL_Pin);
		}
        break;                    // arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:  // arg_int=0: Output low at I2C data pin
        if(arg_int == 1)
		{
			GPIO_SetBits(IIC_GPIO_Port, SDA_Pin);
		}
		else if(arg_int == 0)
		{
			GPIO_ResetBits(IIC_GPIO_Port, SDA_Pin);
		}
        break;                    // arg_int=1: Input dir with pullup high for I2C data pin
    case U8X8_MSG_GPIO_MENU_SELECT:
        u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_NEXT:
        u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_PREV:
        u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_HOME:
        u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
        break;
    default:
        u8x8_SetGPIOResult(u8x8, 1); // default return value
        break;
    }
    return 1;
}

void u8g2Init(u8g2_t *u8g2)
{
	u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay);  // 初始化 u8g2 结构体
	u8g2_InitDisplay(u8g2);         // 根据所选的芯片进行初始化工作，初始化完成后，显示器处于关闭状态
	u8g2_SetPowerSave(u8g2, 0);     // 打开显示器

	u8g2_ClearBuffer(u8g2);         // 清除缓冲区
}

u8g2_t OLED_Structure;  //


void OLED_Init(void)
{
    IIC_Init();

	u8g2Init(&OLED_Structure);
}

/************************************************************************************************************************/


/************************************************************************************************************************/
//各组件封装
/************************************************************************************************************************/

uint8_t Menu_Index = 1;     //菜单索引号

/**
 * @brief  绘制进度条
 * @note   
 * @param  *data: 
 * @retval None
 */
void Progress_bar(uint8_t *data)
{
    uint8_t i;

    if(data ==  NULL)
    {
        for(i = 9; i < 100; i++)
        {
            u8g2_DrawRBox(&OLED_Structure,10,30,i,10,3);//填充框圆角实心矩形框
            u8g2_DrawRFrame(&OLED_Structure,9,29,100,12,4);//空心圆角矩形框
        }
    }
    else
    {
        i = *data;
        if(*data < 6)
        {
            i = 6;
        }
        else if(*data > 100)
        {
            i = 100;
        }
        u8g2_DrawRBox(&OLED_Structure,10,30,i,10,3);//填充框圆角实心矩形框
        u8g2_DrawRFrame(&OLED_Structure,9,29,100,12,4);//空心圆角矩形框
    }
}

/**
 * @brief  调用菜单的功能函数
 * @note   
 * @retval None
 */
uint8_t Menu_Func_DataBuff[28] = {0};
float Menu_Func_Data[14] = {1,1};
void Menu_Func(u8g2_t *u8g2,  Menu_Table *Menu)
{
        static uint8_t now_page = 0, last_page = 0;
        uint8_t str_w, i;

        i = 1;

        last_page = now_page;
        if(Menu_Index > 3)
        {
            now_page = Menu_Index - 3;
            if(now_page > last_page);
            else if(now_page < last_page && Menu_Index - last_page >= 1)
            {
                now_page = last_page;
            }
        }
        else
        {
            now_page = 0;
        }

        Menu[0].func1();

        while(Menu[i].fatherMenu != NULL)
        {
            str_w = u8g2_GetStrWidth(u8g2, Menu[i].menu_name);

            if((i - now_page) > 0)
            {
                OLED_printf(6 + str_w , 12 + (i-now_page)*16, u8g2_font_ncenB10_tf, 1, "%.2f", Menu_Func_Data[i - 1]);
                Menu_Func_Data[i - 1] = 0;
            }
            i++;
        }
}

/**
 * @brief  高亮显示菜单
 * @note   
 * @param  *u8g2: U8g2结构体
 * @param  *Menu: 调用菜单
 * @param  Num: 索引号  指定高亮菜单
 * @param  func: 指定调用的功能
 * @retval None
 */
void HightLight(u8g2_t *u8g2,  Menu_Table *Menu, uint8_t Num, uint8_t func)
{
    static uint8_t now_page = 0, last_page = 0;
    uint8_t str_w, str_h, i, title_w = u8g2_GetStrWidth(u8g2,  Menu[0].menu_name) + 2, title_x = (127 - title_w)/2;

    u8g2_FirstPage(u8g2);
    do
    {
        u8g2_ClearBuffer(u8g2);

        u8g2_DrawRBox(u8g2, title_x - 2, 0, title_w + 4, 14, 4);
        OLED_printf(title_x + 1, 12, u8g2_font_ncenB10_tf, 0, Menu[0].menu_name);

        // i = 1 + Menu_Index/4;
        i = 1;

        last_page = now_page;
        if(Menu_Index > 3)
        {
            now_page = Menu_Index - 3;
            if(now_page > last_page);
            else if(now_page < last_page && Menu_Index - last_page >= 1)
            {
                now_page = last_page;
            }
        }
        else
        {
            now_page = 0;
        }

        while(Menu[i].fatherMenu != NULL)
        {
            str_w = u8g2_GetStrWidth(u8g2, Menu[i].menu_name);
            str_h = u8g2_GetAscent(u8g2) - u8g2_GetDescent(u8g2);

            if((i - now_page) > 0)
            {
                if(i == Menu_Index)
                {
                    u8g2_DrawRBox(u8g2, 0, 12 + (i-now_page)*16 - u8g2_GetAscent(u8g2) - 2, str_w + 4, str_h + 2, 4);
                    OLED_printf(2 , 12 + (i-now_page)*16, u8g2_font_ncenB10_tf, 0, Menu[i].menu_name);    //高亮标记
                }
                else
                {
                    u8g2_DrawStr(u8g2, 2, 12 + (i-now_page)*16,Menu[i].menu_name);
                }
            }
            i++;
        }

        if(func == 2)
        {
            Menu_Func(u8g2, Menu);
        }

        u8g2_SendBuffer(u8g2);
    }while (u8g2_NextPage(u8g2));
}

void HightLight_Scroll(u8g2_t *u8g2, Menu_Table *Menu, uint8_t Num)
{
    static uint8_t now_x = 2, now_y = 28, now_w = 10, now_h = 16;
    uint8_t        i = 1 + Menu_Index/4, str_x = 2, str_y = 14, str_w = u8g2_GetStrWidth(u8g2, Menu[Num].menu_name), str_h = u8g2_GetAscent(u8g2) - u8g2_GetDescent(u8g2),
                   title_w = u8g2_GetStrWidth(u8g2, Menu[0].menu_name) + 2, title_x = (127 - title_w)/2;

    u8g2_FirstPage(u8g2);
    do
    {
        u8g2_ClearBuffer(u8g2);
        while(Main_Menu[i].fatherMenu != NULL)
        {
            u8g2_DrawRBox(u8g2, title_x - 1, 0, title_w + 2, 14, 4);
            OLED_printf(title_x + 1, 12, u8g2_font_ncenB10_tf, 0, Menu[0].menu_name);
            // u8g2_DrawHLine(u8g2, 0, 14, 128);

            if(i == Num)
            {
                u8g2_DrawRBox(u8g2, 0, now_y, now_w, str_h + 2, 4);
                OLED_printf(now_x,  12 + (i - Menu_Index/4)*16, u8g2_font_ncenB10_tf, 0, Menu[i].menu_name);

                if(now_y < str_y)
                {
                    now_y += 2;
                }
                else if(now_y > str_y)
                {
                    now_y -= 2;
                }

                if(now_w < str_w + 4)
                {
                    now_w += 2;
                }
                if(now_w > str_w + 4)
                {
                    now_w -= 2;
                }
            }
            else
            {
                OLED_printf(now_x,  12 + (i - Menu_Index/4)*16, u8g2_font_ncenB10_tf, 1, Menu[i].menu_name);
            }
            i++;
            str_y = 14 + (i - Menu_Index/4 - 1)*16;
        }
        u8g2_SendBuffer(u8g2);
        // now_y = 14 + (Menu_Index/4)*16
    }while(u8g2_NextPage(u8g2));
}

/************************************************************************************************************************/


/************************************************************************************************************************/
//菜单设计
/************************************************************************************************************************/
Menu_Table Home_Menu = {"Home", (*HomeUI), NULL, NULL, Main_Menu};

Menu_Table Main_Menu[10] = {
    {"<Main>",  (*MainUI),  NULL, &Home_Menu,   NULL},
    {"Set",     (*SetUI),   NULL, Main_Menu,    Set_Menu},
    {"PID",     (*PIDUI),   NULL, Main_Menu,    PID_Menu},
    {"GAME",    (*GAMEUI),  NULL, Main_Menu,    GAME_Menu},
    {"Gyro",    (*GyroUI),  NULL, Main_Menu,    Gyro_Menu},
    {"Moto",    (*MotoUI),  NULL, Main_Menu,    Moto_Menu},
    {">>",      NULL,       NULL, Main_Menu,    NULL},
    {">>>",     NULL,       NULL, Main_Menu,    NULL},
    {"<",       NULL,       NULL, Main_Menu,    NULL}
};

Menu_Table Set_Menu[10] = {
    {"<Set>",   NULL,   NULL, Set_Menu,  NULL},
    {"Bright",  NULL,   NULL, Set_Menu,  NULL}
};

Menu_Table PID_Menu[10] = {
    {"<PID>",   NULL,    (*PID_Data),     PID_Menu,   NULL},
    {"P:",      (*P_set),NULL,            PID_Menu,   NULL},
    {"I:",      (*I_set),NULL,            PID_Menu,   NULL},
    {"D:",      (*D_set),NULL,            PID_Menu,   NULL}
};

Menu_Table GAME_Menu[10] = {
    {"<GAME>",  NULL, NULL, GAME_Menu,  NULL},
    {"TanChi",  NULL, NULL, GAME_Menu,  NULL}
};

Menu_Table Gyro_Menu[12] = {
    {"<Gyro>",  NULL,(*Gyro_GetData),       Gyro_Menu, NULL},
    {"XAcc:",   NULL,           NULL,       Gyro_Menu, NULL},
    {"YAcc:",   NULL,           NULL,       Gyro_Menu, NULL},
    {"ZAcc:",   NULL,           NULL,       Gyro_Menu, NULL},

    {"XGyro:",  NULL,           NULL,       Gyro_Menu, NULL},
    {"YGyro:",  NULL,           NULL,       Gyro_Menu, NULL},
    {"ZGyro:",  NULL,           NULL,       Gyro_Menu, NULL},

    {"Roll:",   NULL,           NULL,       Gyro_Menu, NULL},
    {"Pitch:",  NULL,           NULL,       Gyro_Menu, NULL},
    {"Yaw:",    NULL,           NULL,       Gyro_Menu, NULL},

    {"Temp:",   NULL,           NULL,       Gyro_Menu, NULL}
};

Menu_Table Moto_Menu[10] = {
    {"<Moto>",      NULL,   (*Get_MotoState),   Moto_Menu, NULL},
    {"L_Speed:",    NULL,   NULL,               Moto_Menu, NULL},
    {"R_Speed:",    NULL,   NULL,               Moto_Menu, NULL},
};

void HomeUI(void)
{
    u8g2_FirstPage(&OLED_Structure);
    do
    {
        for(uint8_t i=9;i<=99;i++)
        {
            u8g2_ClearBuffer(&OLED_Structure);

            u8g2_DrawRBox(&OLED_Structure,29,0,63,13,4);
            OLED_printf(30,12,u8g2_font_ncenB10_tf, 0,"<Home>");    //透明显示标题

            // u8g2_SetDrawColor(&OLED_Structure, 1);  //翻转颜色防止其他UI透明显示

            // u8g2_SetFont(&OLED_Structure, u8g2_font_ncenB10_tf); /*字库选择*/
            // u8g2_SetDrawColor(&OLED_Structure, 1);
            // u8g2_DrawStr(&OLED_Structure,30,11,"<Home>");

            Progress_bar(&i);
            // u8g2_DrawRBox(&OLED_Structure,10,30,i,10,3);//填充框实心矩形框
            // u8g2_DrawRFrame(&OLED_Structure,9,29,100,12,4);//空心矩形框

            u8g2_SendBuffer(&OLED_Structure);
        }
    }while (u8g2_NextPage(&OLED_Structure));
}

void MainUI(void)
{
    // uint8_t i, now_page;

    // u8g2_FirstPage(&OLED_Structure);
    // do
    // {
    //     u8g2_ClearBuffer(&OLED_Structure);

    //     u8g2_DrawRBox(&OLED_Structure,29,0,55,13,4);//绘制标题栏
    //     OLED_printf(30,12,u8g2_font_ncenB10_tf, 0,"<Main>");

    //     // i = 1 + Menu_Index/4;
    //     i = 1;

    //     if(Menu_Index > 3)
    //     {
    //         now_page = Menu_Index - 3;
    //     }
    //     else
    //     {
    //         now_page = 0;
    //     }

    //     while(Main_Menu[i].fatherMenu != NULL)
    //     {
    //         if((i - now_page) > 0)
    //         {
    //             if(i == Menu_Index)
    //             {
    //                 HightLight(&OLED_Structure,2, 12 + (i-now_page)*16, Main_Menu[i].menu_name, i);
    //             }
    //             else
    //             {
    //                 u8g2_DrawStr(&OLED_Structure, 2, 12 + (i-now_page)*16,Main_Menu[i].menu_name);
    //             }
    //         }
    //         i++;
    //     }
    //     u8g2_SendBuffer(&OLED_Structure);
    // }while (u8g2_NextPage(&OLED_Structure));

    HightLight(&OLED_Structure,Main_Menu,Menu_Index,0);

    // HightLight_Scroll(&OLED_Structure, Main_Menu, Menu_Index);
}
void SetUI(void)
{
    HightLight(&OLED_Structure,Set_Menu,Menu_Index,0);
}
void PIDUI(void)
{
    HightLight(&OLED_Structure,PID_Menu,Menu_Index,2);
}
void GAMEUI(void)
{
    HightLight(&OLED_Structure,GAME_Menu,Menu_Index,0);
}
void GyroUI(void)
{
    HightLight(&OLED_Structure,Gyro_Menu,Menu_Index,2);
}

void MotoUI(void)
{
    HightLight(&OLED_Structure,Moto_Menu,Menu_Index,2);
}


void PID_Data(void)
{
    Menu_Func_Data[0] = PID_1.P;
    Menu_Func_Data[1] = PID_1.I;
    Menu_Func_Data[2] = PID_1.D;
}

void P_set(void)
{
    static uint8_t i = 10;

    if(Key_State == 1)
    {
        (PID_1.P) += 0.01;
    }
    else if(Key_State == 2)
    {
        (PID_1.P) -= 0.01;
    }

    if(PID_1.P < 0)
    {
        PID_1.P = 0;
    }
    else if(PID_1.P > 20)
    {
        PID_1.P = 20;
    }

    i = 1+(PID_1.P)*5;

    u8g2_FirstPage(&OLED_Structure);
    do
    {
        u8g2_ClearBuffer(&OLED_Structure);

        u8g2_DrawRFrame(&OLED_Structure,2,2,124,60,4);
        OLED_printf(45,20,u8g2_font_ncenB10_tf, 1,PID_Menu[1].menu_name);
        OLED_printf(60,20,u8g2_font_ncenB10_tf, 1,"%.2f", PID_1.P);

        Progress_bar(&i);

        u8g2_SendBuffer(&OLED_Structure);
    }while (u8g2_NextPage(&OLED_Structure));
}

void I_set(void)
{
    static uint8_t i = 10;

    if(Key_State == 1)
    {
        PID_1.I += 0.01;
    }
    else if(Key_State == 2)
    {
        PID_1.I -= 0.01;
    }

    if(PID_1.I < 0)
    {
        PID_1.I = 0;
    }
    else if(PID_1.I > 20)
    {
        PID_1.I = 20;
    }

    i = 1+(PID_1.I)*5;

    u8g2_FirstPage(&OLED_Structure);
    do
    {
        u8g2_ClearBuffer(&OLED_Structure);

        u8g2_DrawRFrame(&OLED_Structure,2,2,124,60,4);
        OLED_printf(45,20,u8g2_font_ncenB10_tf, 1,PID_Menu[2].menu_name);
        OLED_printf(60,20,u8g2_font_ncenB10_tf, 1,"%.2f", PID_1.I);

        Progress_bar(&i);

        u8g2_SendBuffer(&OLED_Structure);
    }while (u8g2_NextPage(&OLED_Structure));
}

void D_set(void)
{
    static uint8_t i = 10;

    if(Key_State == 1)
    {
        PID_1.D += 0.01;
    }
    else if(Key_State == 2)
    {
        PID_1.D -= 0.01;
    }

    if(PID_1.D < 0)
    {
        PID_1.D = 0;
    }
    else if(PID_1.D > 20)
    {
        PID_1.D = 20;
    }

    i = 1+(PID_1.D)*5;

    u8g2_FirstPage(&OLED_Structure);
    do
    {
        u8g2_ClearBuffer(&OLED_Structure);

        u8g2_DrawRFrame(&OLED_Structure,2,2,124,60,4);
        OLED_printf(45,20,u8g2_font_ncenB10_tf, 1,PID_Menu[3].menu_name);
        OLED_printf(60,20,u8g2_font_ncenB10_tf, 1,"%.2f", PID_1.D);

        Progress_bar(&i);

        u8g2_SendBuffer(&OLED_Structure);
    }while (u8g2_NextPage(&OLED_Structure));
}

void Gyro_GetData(void)
{
    JY901_GetData(Menu_Func_DataBuff);
    JY901_DisposeData(Menu_Func_DataBuff,Menu_Func_Data);
}

void Get_MotoState(void)
{
    static float Left_Speed = 1, Right_Speed = 1;

    PID_1.Target = 80;
    PID_2.Target = 20;

    Left_Speed = PID_PosOperation(&PID_1,Left_Speed);
    Right_Speed = PID_PosOperation(&PID_2,Right_Speed);

    Menu_Func_Data[0] = L_MotoState(Left_Speed,1)/100.0;
    Menu_Func_Data[1] = R_MotoState(Right_Speed,1)/100.0;
}

/**
 * @brief  刷新菜单
 * @note   
 * @retval None
 */
void RefreshMenue(void)
{
    static uint8_t Page_State = 0, Item_State = 0,First_Index = 1, Last_Index = 10, UI_Func_State = 0;

    Key_State = KeyScan();

    if(Page_State == 0)
    {
        // HomeUI();
        Page_State = 1;
        MainUI();
    }
    else
    {
        if(Key_State == 1)
        {
            if(UI_Func_State == 0)
            {
                Menu_Index++;
            }
            if(Page_State == 1)
            {
                if(Main_Menu[Menu_Index].fatherMenu == NULL)
                {
                    Last_Index = Menu_Index-1;
                    Menu_Index = First_Index;
                }
            }
            else
            {
                if(Main_Menu[Item_State].childMenu[Menu_Index].fatherMenu == NULL)
                {
                    Last_Index = Menu_Index-1;
                    Menu_Index = First_Index;
                }
            }
        }
        else if(Key_State == 2)
        {
            if(UI_Func_State == 0)
            {
                Menu_Index--;
            }
            if(Menu_Index == 0)
            {
                First_Index = Menu_Index-1;
                Menu_Index = Last_Index;
            }
        }
        else if(Key_State == 3)
        {
            if(Page_State == 1)
            {
                if(Main_Menu[Menu_Index].childMenu != NULL)
                {
                    Page_State++;
                    Item_State = Menu_Index;
                    Menu_Index = 1;
                }
            }
            else
            {
                if(Main_Menu[Item_State].childMenu[Menu_Index].childMenu != NULL)
                {
                    Page_State++;
                    Item_State = Menu_Index;
                    Menu_Index = 1;
                }
                if(Main_Menu[Item_State].childMenu[Menu_Index].childMenu == NULL && Main_Menu[Item_State].childMenu[Menu_Index].UIfunc != NULL)
                {
                    Page_State++;
                    UI_Func_State = 1;
                }
            }
        }
        else if(Key_State == 4)
        {
            if(Page_State > 1)
            {
                if(Main_Menu[Item_State].childMenu[Menu_Index].fatherMenu != NULL && Main_Menu[Item_State].childMenu[Menu_Index].UIfunc != NULL)
                {
                    Page_State--;
                    UI_Func_State = 0;
                }
                else if(Main_Menu[Item_State].childMenu[Menu_Index].fatherMenu != NULL)
                {
                    Page_State--;
                    Menu_Index = Item_State;
                }
            }
            if(Page_State == 1)
            {
                if(Main_Menu[Menu_Index].fatherMenu != NULL)
                {
                    Page_State--;
                    Menu_Index = Item_State;
                    Item_State = 0;
                }
            }
        }

        if(UI_Func_State == 0)
        {
            Main_Menu[Item_State].UIfunc();
        }
        else
        {
            Main_Menu[Item_State].childMenu[Menu_Index].UIfunc();
        }
    }
}

/************************************************************************************************************************/


/************************************************************************************************************************/
//测试
/************************************************************************************************************************/