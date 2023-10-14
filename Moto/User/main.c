#include "stm32f10x.h"                  // Device header
#include "PWM.h"

int main(void)
{
	PWM_Init();

	PWM_SetComPare1(20);
	PWM_SetComPare2(40);
	PWM_SetComPare3(60);
	PWM_SetComPare4(80);
	while(1)
	{
		
	}
}
