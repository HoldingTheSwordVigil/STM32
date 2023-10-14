#include "OLED.h"
#include "OLED_Font.h"

/*引脚配置*/
#define OLED_SDA_IN()    {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=0x00008000;}
#define OLED_SDA_OUT()   {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=0x00003000;}

#define OLED_IIC_SCL     PBout(8) //SCL
#define OLED_IIC_SDA     PBout(9) //SDA
#define OLED_READ_SDA    PBin(8)

/*引脚初始化*/
void OLED_IIC_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

    OLED_SDA_OUT();

	OLED_IIC_SCL = 1;
	OLED_IIC_SDA = 1;
}

/**
 * @brief  OLED IIC通讯开始
 * @note   
 * @retval None
 */
void OLED_IIC_Start(void)
{
    OLED_SDA_OUT();

    OLED_IIC_SDA = 1;
    OLED_IIC_SCL = 1;
    Delay_us(1);
    OLED_IIC_SDA = 0;
    Delay_us(1);
    OLED_IIC_SCL = 0;
}

/**
 * @brief  OLED IIC通讯停止
 * @note   
 * @retval None
 */
void OLED_IIC_Stop(void)
{
    OLED_SDA_OUT();

    OLED_IIC_SDA = 0;
    Delay_us(1);
    OLED_IIC_SCL = 1;
    Delay_us(1);
    OLED_IIC_SDA = 1;
    Delay_us(1);
}

/**
 * @brief  向OLED发送一个字节数据
 * @note   
 * @param  Byte: 要发送的一个字节数据
 * @retval None
 */
void OLED_IIC_SendByte(uint8_t Byte)
{
    OLED_SDA_OUT();

    for (uint8_t i = 0; i < 8; i++)
    {
        OLED_IIC_SDA = (Byte & 0x80) >> 7;
        Byte <<= 1;
        OLED_IIC_SCL = 1;
        Delay_us(1);
        OLED_IIC_SCL = 0;
        Delay_us(1);
    }
    OLED_IIC_SCL = 1;       //额外一个时钟，不处理应答信号
    Delay_us(1);
    OLED_IIC_SCL = 0;
}

/**
 * @brief  向OLED写命令
 * @note   
 * @param  Command: 写入的命令
 * @retval None
 */
void OLED_WriteCommand(uint8_t Command)
{
    OLED_IIC_Start();
    OLED_IIC_SendByte(0x78);        //从机地址
    OLED_IIC_SendByte(0x00);        //写命令
    OLED_IIC_SendByte(Command);
    OLED_IIC_Stop();
}

/**
  * @brief  向OLED写数据
  * @param  Data: 要写入的数据
  * @retval None
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_IIC_Start();
	OLED_IIC_SendByte(0x78);		//从机地址
	OLED_IIC_SendByte(0x40);		//写数据
	OLED_IIC_SendByte(Data);
	OLED_IIC_Stop();
}

/**
  * @brief  OLED设置光标位置
  * @param  Y: 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X: 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval None
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}

/**
 * @brief  OLED全屏填充
 * @note   
 * @param  Fill_Data: 填充的值
 * @retval None
 */
void OLED_Fill(uint8_t Fill_Data)
{
    uint8_t x,y;

    for(x = 0;x < 8;x++)
    {
        //设置光标位置
        OLED_WriteCommand(0xb0 + x);
        OLED_WriteCommand(0x00);
        OLED_WriteCommand(0x10);

        for(y = 0;y < 128;y++)
        {
            OLED_WriteData(Fill_Data);
        }
    }
}

/**
  * @brief  OLED清屏
  * @param  None
  * @retval None
  */
void OLED_Clear(void)
{
    OLED_Fill(0x00);
}

/**
 * @brief  开启OLED
 * @note   
 * @retval None
 */
void OLED_ON(void)   //打开OLED
{
	OLED_WriteCommand(0X8D);   //设置电荷泵
	OLED_WriteCommand(0X14);   //开启电荷泵
	OLED_WriteCommand(0XAF);   //OLED唤醒
}

/**
 * @brief  关闭OLED
 * @note   
 * @retval None
 */
void OLED_OFF(void)  //关闭OLED
{
	OLED_WriteCommand(0X8D);   //设置电荷泵
	OLED_WriteCommand(0X10);   //关闭电荷泵
	OLED_WriteCommand(0XAE);   //关闭OLED
}

/**
 * @brief  OLED显示一个字符
 * @note   
 * @param  Line: 指定行位置，范围：1~8；1~4
 * @param  Column: 指定列位置，范围：1~21；1~16；1~8
 * @param  Char: 要显示的字符，范围：ASCII可见字符
 * @param  Size: 指定字符的大小
 * @param  Inverse: 显示反色；0：正常显示，1：反色显示
 * @retval None
 */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char, uint16_t Size, uint8_t Inverse)
{
    uint8_t i;

    if(Size == 806)
    {
        OLED_SetCursor(Line - 1, (Column - 1) * 6);

        for(i = 0;i < 8;i++)
        {
            if(Inverse == 0)
            {
                OLED_WriteData(OLED_F6x8[Char - 32][i]);
            }
            else
            {
                OLED_WriteData(~OLED_F6x8[Char - 32][i]);
            }
        }
    }
    else if(Size == 1608)
    {
        OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);
        for(i = 0;i < 8;i++)
        {
            if(Inverse == 0)
            {
                OLED_WriteData(OLED_F8x16[Char - 32][i]);
            }
            else
            {
                OLED_WriteData(~OLED_F8x16[Char - 32][i]);
            }
        }

        OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);
        for(i = 0;i < 8;i++)
        {
            if(Inverse == 0)
            {
                OLED_WriteData(OLED_F8x16[Char - 32][i + 8]);
            }
            else
            {
                OLED_WriteData(~OLED_F8x16[Char - 32][i + 8]);
            }
        }
    }
    else if(Size == 1616)
    {
        OLED_SetCursor((Line - 1) * 2, (Column - 1) * 16);
        for(i = 0;i < 16;i++)
        {
            if(Inverse == 0)
            {
                OLED_WriteData(OLED_F16x16[Char - 32][i]);
            }
            else
            {
                OLED_WriteData(~OLED_F16x16[Char - 32][i]);
            }
        }

        OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 16);
        for(i = 0;i < 16;i++)
        {
            if(Inverse == 0)
            {
                OLED_WriteData(OLED_F16x16[Char - 32][i + 16]);
            }
            else
            {
                OLED_WriteData(~OLED_F16x16[Char - 32][i + 16]);
            }
        }
    }
}

/**
 * @brief  OLED显示字符串
 * @note   
 * @param  Line: 指定行位置，范围：1~8；1~4
 * @param  Column: 指定列位置，范围：1~21；1~16；1~8
 * @param  *String: 要显示的字符串
 * @param  Size: 指定字符的大小
 * @param  Inverse: 显示反色；0：正常显示，1：反色显示
 * @retval None
 */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String, uint16_t Size, uint8_t Inverse)
{
    uint8_t i;

	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i],Size,Inverse);
	}
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 指定行位置，范围：1~8；1~4
  * @param  Column 指定列位置，范围：1~21；1~16；1~8
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @param  Size: 指定字符的大小
  * @param  Inverse: 显示反色；0：正常显示，1：反色显示
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint16_t Size, uint8_t Inverse)
{
	uint8_t i;
	for (i = 0; i < Length; i++)
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0',Size,Inverse);
	}
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 指定行位置，范围：1~8；1~4
  * @param  Column 指定列位置，范围：1~21；1~16；1~8
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @param  Size: 指定字符的大小
  * @param  Inverse: 显示反色；0：正常显示，1：反色显示
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length, uint16_t Size, uint8_t Inverse)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+',Size,Inverse);
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-',Size,Inverse);
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0',Size,Inverse);
	}
}

/**
 * @brief  OLED显示十六进制数字
 * @note   
 * @param  Line: 指定行位置，范围：1~8；1~4
 * @param  Column: 指定列位置，范围：1~21；1~16；1~8
 * @param  Number: 要显示的数字，范围：0~0xFFFFFFFF
 * @param  Length: 要显示数字的长度
 * @param  Size: 指定字符的大小
 * @param  Inverse: 显示反色；0：正常显示，1：反色显示
 * @retval None
 */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint16_t Size, uint8_t Inverse)
{
    uint32_t i,SingleNumber;

    for(i = 0;i < Length;i++)
    {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;

        if(SingleNumber < 10)
        {
            OLED_ShowChar(Line, Column + i, SingleNumber + '0',Size,Inverse);
        }
        else
        {
            OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A',Size,Inverse);
        }
    }
}

/**
 * @brief  OLED显示二进制数字
 * @note   
 * @param  Line: 指定行位置，范围：1~8；1~4
 * @param  Column: 指定列位置，范围：1~21；1~16；1~8
 * @param  Number: 要显示的数字，范围：0~1111 1111 1111 1111
 * @param  Length: 要显示数字的长度
 * @param  Size: 指定字符的大小
 * @param  Inverse: 显示反色；0：正常显示，1：反色显示
 * @retval None
 */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint16_t Size, uint8_t Inverse)
{
	uint8_t i;
	for (i = 0; i < Length; i++)
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0',Size,Inverse);
	}
}

/**
 * @brief  OLED字符水平左右滚动
 * @note   
 * @param  LineMin: 指定起始页
 * @param  LineMax: 指定结束页
 * @param  dir: 方向：1，向右滚动；2，向左滚动
 * @param  Speed: 滚动速度
 *              设置每个滚动步骤之间的时间间隔的帧频
                000 b - 5帧     100 b - 3帧
                001 b - 64帧    101 b - 4帧
                010 b - 128帧   110 b - 25帧
                011 b - 256帧   111 b - 2帧
 * @retval None
 */
void OLED_LevelRun(uint8_t LineMin, uint8_t LineMax, uint8_t dir, uint8_t Speed)
{
    OLED_WriteCommand(0x2E);                //关闭滚动
    OLED_WriteCommand(0x25 + dir);          //水平向右或者左滚动 26/27
    OLED_WriteCommand(0x00);                //虚拟字节
    OLED_WriteCommand(0x00 + LineMin);      //起始页
    OLED_WriteCommand(0x00 + Speed);        //滚动时间间隔
    OLED_WriteCommand(0x00 + LineMax);      //终止页
    OLED_WriteCommand(0x00);                //虚拟字节
    OLED_WriteCommand(0xFF);                //虚拟字节
    OLED_WriteCommand(0x2F);                //开启滚动
}

void OLED_VTOLRun(uint8_t LineMin, uint8_t LineMax, uint8_t Speed)
{
    OLED_WriteCommand(0x2E);                //关闭滚动
    OLED_WriteCommand(0x29);          //水平垂直和水平滚动左右 29/2a
    OLED_WriteCommand(0x00);                //虚拟字节
    OLED_WriteCommand(0x00 + LineMin);      //起始页
    OLED_WriteCommand(0x00 + Speed);        //滚动时间间隔
    OLED_WriteCommand(0x00 + LineMax);      //终止页
    OLED_WriteCommand(0x01);                //垂直滚动偏移量
    OLED_WriteCommand(0x2F);                //开启滚动
}

void OLED_Init(void)
{
    uint16_t i, j;

	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}

    OLED_IIC_Init();		 //端口初始化

    OLED_WriteCommand(0xAE); //关闭显示

	OLED_WriteCommand(0xD5); //设置时钟分频因子,震荡频率
	OLED_WriteCommand(0x80); //[3:0],分频因子;[7:4],震荡频率

	OLED_WriteCommand(0xA8); //设置多路复用率
	OLED_WriteCommand(0X3F); //默认0X3F(1/64)

	OLED_WriteCommand(0xD3); //设置显示偏移
	OLED_WriteCommand(0X00); //默认为0

	OLED_WriteCommand(0x40); //设置显示开始行 [5:0],行数.

    OLED_WriteCommand(0xA1); //设置左右方向，0xA1正常 0xA0左右反置

	OLED_WriteCommand(0xC8); //设置上下方向，0xC8正常 0xC0上下反置

    OLED_WriteCommand(0xDA); //设置COM硬件引脚配置
	OLED_WriteCommand(0x12); //[5:4]配置

    OLED_WriteCommand(0x81); //对比度设置
	OLED_WriteCommand(0xEF); //1~255;默认0X7F (亮度设置,越大越亮)

    OLED_WriteCommand(0xD9); //设置预充电周期
	OLED_WriteCommand(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;

    OLED_WriteCommand(0xDB); //设置VCOMH 电压倍率
	OLED_WriteCommand(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

    OLED_WriteCommand(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)

    OLED_WriteCommand(0xA6); //设置显示方式;bit0:1,反相显示;0,正常显示

	OLED_WriteCommand(0x8D); //电荷泵设置
	OLED_WriteCommand(0x14); //bit2，开启/关闭

	OLED_WriteCommand(0x20); //设置内存地址模式
	OLED_WriteCommand(0x02); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;

	OLED_WriteCommand(0xAF); //开启显示

	OLED_Clear();            //OLED清屏
}

/**********************************************************************************
 * 扩展部分
***********************************************************************************/

//OLED显存
uint8_t OLED_GRAM[8][128] = {0x00};

/**
 * @brief  填充OLED显存
 * @note   
 * @param  *Data: 要填充的数据
 * @param  X_Start: 填充的x起点，范围：0~127
 * @param  Y_Start: 填充的y起点，范围：0~63
 * @param  Width: 填充数据一行的宽度
 * @param  Num: 填充的数据总量
 * @retval None
 */
void GRAM_Value(const uint8_t *Data, uint8_t X_Start, uint8_t Y_Start, uint16_t Width, uint32_t Num)
{
    uint8_t i;
    uint32_t cnt;

    cnt = Num;

    for(cnt = 0; cnt < Num;)
    {
        for(i = 0; i < Width; i++)
        {
            OLED_GRAM[(Y_Start / 8) + (cnt / Width)][X_Start + i] = Data[cnt];
            cnt++;
        }
    }
}

/**
 * @brief  清空OLED显存
 * @note   
 * @retval None
 */
void Clear_GRAM(void)
{
    uint8_t i, j;

    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 128; j++)
        {
            OLED_GRAM[i][j] = 0x00;
        }
    }
}

/**
 * @brief  刷新OLED显存
 * @note   
 * @retval None
 */
void OLED_Refresh_GRAM(void)
{
    uint8_t x,y;

    for(y = 0;y < 8;y++)
    {
        OLED_WriteCommand(0xB0 | y);
        OLED_WriteCommand(0x10);
        OLED_WriteCommand(0x00);

        for(x = 0;x < 128;x++)
        {
            OLED_WriteData(OLED_GRAM[y][x]);
        }
    }
}

/**
 * @brief  OLED设置点位
 * @note   
 * @param  X: 以左上角为原点，向右方向的坐标，范围：0~127
 * @param  Y: 以左上角为原点，向下方向的坐标，范围：0~63
 * @retval None
 */
void OLED_SetPoint(uint8_t X,uint8_t Y)
{
    OLED_GRAM[Y / 8][X] |= 0x01 << (Y % 8);
}

/**
 * @brief  OLED画点
 * @note   
 * @param  X: 以左上角为原点，向右方向的坐标，范围：0~127
 * @param  Y: 以左上角为原点，向下方向的坐标，范围：0~63
 * @retval None
 */
void OLED_DrawPoint(uint8_t X,uint8_t Y)
{
    OLED_GRAM[Y / 8][X] |= 0x01 << (Y % 8);

	OLED_WriteCommand(0xB0 | (Y / 8));					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位

    OLED_WriteData(OLED_GRAM[Y / 8][X]);
}

/**
 * @brief  OLED画线函数
 * @note   
 * @param  x1: 第一个点的x坐标，范围：0~127
 * @param  y1: 第一个点的y坐标，范围：0~63
 * @param  x2: 第二个点的x坐标，范围：0~127
 * @param  y2: 第二个点的y坐标，范围：0~63
 * @retval None
 */
void OLED_DrawLine(uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2)
{
    double k, dx, dy, x, y, xEnd, yEnd;

    dx = x2 - x1;
    dy = y2 - y1;

    if(fabs(dx) >= fabs(dy))
    {
        k = dy / dx;

        if(dx > 0)
        {
            x = x1;
            y = y1;
            xEnd = x2;
        }
        else
        {
            x = x2;
            y = y2;
            xEnd = x1;
        }

        while(x <= xEnd)
        {
            OLED_SetPoint((uint8_t)x,(uint8_t)round(y));
            y = y + k;
            x = x + 1;
            OLED_Refresh_GRAM();
        }
    }
    else
    {
        k = dx / dy;

        if(dy > 0)
        {
            x = x1;
            y = y1;
            yEnd = y2;
        }
        else
        {
            x = x2;
            y = y2;
            yEnd = y1;
        }

        while(y <= yEnd)
        {
            OLED_SetPoint((uint8_t)round(x),(uint8_t)y);
            x = x + k;
            y = y + 1;
            OLED_Refresh_GRAM();
        }
    }
}

/**
 * @brief  根据平面直角坐标系画线
 * @note   
 * @param  X1: 第一个点的x坐标，范围：-63~64
 * @param  Y1: 第一个点的y坐标，范围：-31~32
 * @param  X2: 第二个点的x坐标，范围：-63~64
 * @param  Y2: 第二个点的y坐标，范围：-31~32
 * @retval None
 */
void OLED_MathLine(int8_t X1, int8_t Y1,int8_t X2, int8_t Y2)
{
    uint8_t x1, y1, x2, y2;

    x1 = 63 + X1;
    y1 = 31 - Y1;
    x2 = 63 + X2;
    y2 = 31 - Y2;

    OLED_DrawLine(x1,y1,x2,y2);
}

uint8_t NowIndex = 0;
/*当前菜单名，当前菜单应执行的操作1，当前菜单可执行的操作2，父级菜单，子级菜单*/
Menu_Table Home_Menu = {"Home", (*ShowMain), NULL, NULL, NULL};

Menu_Table Main_Menu[10] = {
    {"Set",     (*ShowSet), NULL, &Home_Menu, Set_Menu},
    {"PID",     (*ShowPID), NULL, &Home_Menu, PID_Menu},
    {"GAME",    (*ShowGAME), NULL, &Home_Menu, GAME_Menu},
    {"Gyro",    (*ShowGyro), NULL, &Home_Menu, Gyro_Menu}
};

Menu_Table Set_Menu[10] = {
    {"Bright",  NULL, NULL, Main_Menu, NULL}
};

Menu_Table PID_Menu[10] = {
    {"P:",  NULL, NULL, Main_Menu, NULL},
    {"I:",  NULL, NULL, Main_Menu, NULL},
    {"D:",  NULL, NULL, Main_Menu, NULL}
};

Menu_Table GAME_Menu[10] = {
    {"TanChi",  NULL, NULL, Main_Menu, NULL}
};

Menu_Table Gyro_Menu[10] = {
    {"XAcc:",   NULL, NULL, Main_Menu, NULL},
    {"YAcc:",   NULL, NULL, Main_Menu, NULL},
    {"ZAcc:",   NULL, NULL, Main_Menu, NULL},

    {"XGyro:",  NULL, NULL, Main_Menu, NULL},
    {"YGyro:",  NULL, NULL, Main_Menu, NULL},
    {"ZGyro:",  NULL, NULL, Main_Menu, NULL},

    {"Roll:",   NULL, NULL, Main_Menu, NULL},
    {"Pitch:",  NULL, NULL, Main_Menu, NULL},
    {"Yaw:",    NULL, NULL, Main_Menu, NULL},

    {"Temp:",   NULL, NULL, Main_Menu, NULL}
};

void ShowHome(void)
{
    OLED_ShowString(1,3,Home_Menu.Show_Name,1616,0);
}

void ShowMain(void)
{
    if(NowIndex <= 4)
    {
        OLED_ShowString(1,1,Main_Menu[0].Show_Name,1608,0);
        OLED_ShowString(2,1,Main_Menu[1].Show_Name,1608,0);
        OLED_ShowString(3,1,Main_Menu[2].Show_Name,1608,0);
        OLED_ShowString(4,1,Main_Menu[3].Show_Name,1608,0);
    }
    else if(NowIndex > 4)
    {
        OLED_ShowString(1,1,Main_Menu[NowIndex - 3].Show_Name,1608,0);
        OLED_ShowString(2,1,Main_Menu[NowIndex - 2].Show_Name,1608,0);
        OLED_ShowString(3,1,Main_Menu[NowIndex - 1].Show_Name,1608,0);
        OLED_ShowString(4,1,Main_Menu[NowIndex].Show_Name,1608,0);
    }
}

void ShowSet(void)
{
    if(NowIndex <= 4)
    {
        OLED_ShowString(1,1,Set_Menu[0].Show_Name,1608,0);
        OLED_ShowString(2,1,Set_Menu[1].Show_Name,1608,0);
        OLED_ShowString(3,1,Set_Menu[2].Show_Name,1608,0);
        OLED_ShowString(4,1,Set_Menu[3].Show_Name,1608,0);
    }
    else if(NowIndex > 4)
    {
        OLED_ShowString(1,1,Set_Menu[NowIndex - 3].Show_Name,1608,0);
        OLED_ShowString(2,1,Set_Menu[NowIndex - 2].Show_Name,1608,0);
        OLED_ShowString(3,1,Set_Menu[NowIndex - 1].Show_Name,1608,0);
        OLED_ShowString(4,1,Set_Menu[NowIndex].Show_Name,1608,0);
    }
}

void ShowPID(void)
{
    if(NowIndex <= 4)
    {
        OLED_ShowString(1,1,PID_Menu[0].Show_Name,1608,0);
        OLED_ShowString(2,1,PID_Menu[1].Show_Name,1608,0);
        OLED_ShowString(3,1,PID_Menu[2].Show_Name,1608,0);
        OLED_ShowString(4,1,PID_Menu[3].Show_Name,1608,0);
    }
    else if(NowIndex > 4)
    {
        OLED_ShowString(1,1,PID_Menu[NowIndex - 3].Show_Name,1608,0);
        OLED_ShowString(2,1,PID_Menu[NowIndex - 2].Show_Name,1608,0);
        OLED_ShowString(3,1,PID_Menu[NowIndex - 1].Show_Name,1608,0);
        OLED_ShowString(4,1,PID_Menu[NowIndex].Show_Name,1608,0);
    }
}

void ShowGAME(void)
{
    if(NowIndex <= 4)
    {
        OLED_ShowString(1,1,GAME_Menu[0].Show_Name,1608,0);
        OLED_ShowString(2,1,GAME_Menu[1].Show_Name,1608,0);
        OLED_ShowString(3,1,GAME_Menu[2].Show_Name,1608,0);
        OLED_ShowString(4,1,GAME_Menu[3].Show_Name,1608,0);
    }
    else if(NowIndex > 4)
    {
        OLED_ShowString(1,1,GAME_Menu[NowIndex - 3].Show_Name,1608,0);
        OLED_ShowString(2,1,GAME_Menu[NowIndex - 2].Show_Name,1608,0);
        OLED_ShowString(3,1,GAME_Menu[NowIndex - 1].Show_Name,1608,0);
        OLED_ShowString(4,1,GAME_Menu[NowIndex].Show_Name,1608,0);
    }
}

void ShowGyro(void)
{
    if(NowIndex <= 4)
    {
        OLED_ShowString(1,1,Gyro_Menu[0].Show_Name,1608,0);
        OLED_ShowString(2,1,Gyro_Menu[1].Show_Name,1608,0);
        OLED_ShowString(3,1,Gyro_Menu[2].Show_Name,1608,0);
        OLED_ShowString(4,1,Gyro_Menu[3].Show_Name,1608,0);
    }
    else if(NowIndex > 4)
    {
        OLED_ShowString(1,1,Gyro_Menu[NowIndex - 3].Show_Name,1608,0);
        OLED_ShowString(2,1,Gyro_Menu[NowIndex - 2].Show_Name,1608,0);
        OLED_ShowString(3,1,Gyro_Menu[NowIndex - 1].Show_Name,1608,0);
        OLED_ShowString(4,1,Gyro_Menu[NowIndex].Show_Name,1608,0);
    }
}

void Menu_Sing(uint8_t num)
{

}

void RefreshMenue(void)
{
    static uint8_t MenuState = 0, StateFlag = 0;

    if(Key_Flag == 3 && MenuState == 0)
    {
        OLED_Clear();
        Home_Menu.func1();
        MenuState = 1;
        Key_Flag = 0;
    }

    if(MenuState == 1)
    {
        if(Key_Flag == 1)
        {
            NowIndex++;
        }
        else if(Key_Flag == 2)
        {
            NowIndex--;
        }
        else if(Key_Flag == 3)
        {
            OLED_Clear();
            StateFlag = NowIndex;
            NowIndex = 0;
            Main_Menu[StateFlag].func1();
            MenuState = 2;
        }
        else if(Key_Flag == 4)
        {
            OLED_Clear();
            MenuState = 0;
            NowIndex = 0;
            ShowHome();
        }
        Key_Flag = 0;
    }
    else if(MenuState == 2)
    {
        if(Key_Flag == 1)
        {
            NowIndex++;
            OLED_Clear();
            Main_Menu[StateFlag].func1();
        }
        else if(Key_Flag == 2)
        {
            NowIndex--;
            OLED_Clear();
            Main_Menu[StateFlag].func1();
        }
        else if(Key_Flag == 3)
        {
            if(Main_Menu[StateFlag].childMenu[NowIndex].func2 != NULL)
            {
                OLED_Clear();
                Main_Menu[StateFlag].childMenu[NowIndex].func2();
                MenuState = 3;
            }
        }
        else if(Key_Flag == 4)
        {
            OLED_Clear();
            MenuState = 1;
            NowIndex = 0;
            Home_Menu.func1();
        }
        Key_Flag = 0;
    }
}
