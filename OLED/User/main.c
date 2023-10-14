#include "OLED.h"

int main(void)
{
    OLED_Init();
    Key_Init();
    MY_IIC_Init();
    Serial_Vofa_Init();
	Moto_Init();

    Vofa_InitTypeDef Vofa_InitStructure;
	Vofa_InitStructure.Vofa_ChannelNum = Vofa_Channel_3;
	Vofa_InitStructure.Vofa_ChannelVale = &PID_1.P;
	VofaChannel_Init(&Vofa_InitStructure);
	Vofa_InitStructure.Vofa_ChannelNum = Vofa_Channel_4;
	Vofa_InitStructure.Vofa_ChannelVale = &PID_1.I;
	VofaChannel_Init(&Vofa_InitStructure);
	Vofa_InitStructure.Vofa_ChannelNum = Vofa_Channel_5;
	Vofa_InitStructure.Vofa_ChannelVale = &PID_1.D;
	VofaChannel_Init(&Vofa_InitStructure);

	while(1)
	{
        // Serial_Send_Vofa(5);
        RefreshMenue();
		// Draw_test(&OLED_Structure);
    }
}
