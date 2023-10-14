#include "stm32f10x.h"                  // Device header
#include "VOFA_Serial.h"
#include "MPU6050.h"
#include "Vofa.h"
#include "JY901.h"
#include "OLED.h"

uint8_t MPUData[14] = {0};
uint8_t JY901Data[26] = {0};
float TData[13] = {0};

/***********************************贪吃蛇*********************************************/
int8_t TanChi[100][2] = {
	63,31,
	62,31,
	61,31,
	60,31,
	59,31,
	58,31,
	57,31
};
int8_t LR_Mode = 0, LR_Flag = 0;

void Resresh_TanChi(void)
{
	uint8_t y = 0;
	Clear_GRAM();
	while(TanChi[y][0])
	{
		OLED_DrawPoint(TanChi[y][0], TanChi[y][1]);
		y++;
	}
	OLED_Refresh_GRAM();
}

void key(void)
{
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) == 0)
	{
		LR_Flag = -1;
		LR_Mode = -1;
	}
	else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15) == 0)
	{
		LR_Flag = 1;
		LR_Mode = 1;
	}
}
void control(void)
{
	uint8_t y = 0, xdata = TanChi[y][0], ydata = TanChi[y][1];
	int8_t dx = 0, dy = 0;
	dx = 0;
	dy = 0;
	Resresh_TanChi();
	key();
	xdata = TanChi[y][0];
	ydata = TanChi[y][1];
	if(LR_Flag != 0)
	{
		if(xdata == TanChi[y+1][0])
		{
			if(TanChi[y][1] - TanChi[y+1][1] == -1)
			{
				TanChi[y][0] += LR_Mode;
			}
			else if(TanChi[y][1] - TanChi[y+1][1] == 1)
			{
				TanChi[y][0] -= LR_Mode;
			}
		}
		else if(ydata == TanChi[y+1][1])
		{
			if(TanChi[y][0] - TanChi[y+1][0] == -1)
			{
				TanChi[y][1] -= LR_Mode;
			}
			else if(TanChi[y][0] - TanChi[y+1][0] == 1)
			{
				TanChi[y][1] += LR_Mode;
			}
		}
	}
	if(xdata == TanChi[y+1][0])
	{
		dy = TanChi[y][1] - TanChi[y+1][1];
	}
	else if(ydata == TanChi[y+1][1])
	{
		dx = TanChi[y][0] - TanChi[y+1][0];
	}

	while(TanChi[y][0])
	{
		// xdata = TanChi[y][0];
		// ydata = TanChi[y][1];
		if(y == 0)
		{
			if(LR_Flag == 0)
			{
				TanChi[y][0]=TanChi[y][0]+dx;
				TanChi[y][1]=TanChi[y][1]+dy;
			}
		}
		else if(TanChi[y+1][0] == 0)
		{
			TanChi[y][0]=TanChi[y][0]+(TanChi[y-1][0] - TanChi[y][0]);
			TanChi[y][1]=TanChi[y][1]+(TanChi[y-1][1] - TanChi[y][1]);
		}
		else
		{
			xdata = TanChi[y][0];
			ydata = TanChi[y][1];
			TanChi[y][0]=TanChi[y][0]+dx;
			TanChi[y][1]=TanChi[y][1]+dy;
			dx = xdata - TanChi[y+1][0];
			dy = ydata - TanChi[y+1][1];
		}
		Resresh_TanChi();
		// TanChi[y+1][0] = xdata;
		// TanChi[y+1][1] = ydata;
		y++;
	}
	LR_Flag = 0;
}
/*************************************************************************************/

int main(void)
{
	Delay_s(2);

	Serial_Vofa_Init();
	MY_IIC_Init();
	OLED_Init();
	MPU6050_Init();

	// OLED_DrawPoint(61,32);
	// OLED_DrawPoint(62,32);
	// OLED_DrawPoint(63,32);
	// OLED_DrawPoint(64,32);
	// OLED_DrawPoint(64,31);
	// OLED_DrawPoint(64,30);
	// OLED_DrawPoint(64,29);
	// OLED_DrawPoint(64,26);

	// OLED_Refresh_GRAM();


	// for(int8_t i = 0; i < 3; i++)
	// {
	// 	OLED_MathLine(i*i, round(sqrt(9-(i*i))), (i+1)*(i+1), round(sqrt(9-(i+1)*(i+1))));
	// 	OLED_MathLine(i*i, -round(sqrt(9-(i*i))), (i+1)*(i+1), -round(sqrt(9-(i+1)*(i+1))));
	// 	OLED_MathLine(-i*i, -round(sqrt(9-(i*i))), -(i+1)*(i+1), -round(sqrt(9-(i+1)*(i+1))));
	// 	OLED_MathLine(-i*i, round(sqrt(9-(i*i))), -(i+1)*(i+1), round(sqrt(9-(i+1)*(i+1))));
	// }

/***********************************************************/
	Key_Init();
/***********************************************************/

	OLED_Refresh_GRAM();

	ShowHome();
	while(1)
	{
		KeyScan();
		RefreshMenue();
/*********************贪吃蛇函数*****************************/
		// control();
/***********************************************************/

/*********************陀螺仪测试****************************/
		// MPU6050_GetData(MPUData);
		// JY901_GetData(JY901Data);

		// JY901_DisposeData(JY901Data,TData);
		// Vofa_justFloat(TData,9);
/**********************************************************/
	}
}
