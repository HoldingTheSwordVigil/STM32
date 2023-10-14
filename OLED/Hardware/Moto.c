#include "Moto.h"

void Moto_Init(void)
{
    PWM_Init();
}

uint16_t L_MotoState(uint16_t Speed, uint8_t Dir)
{
    uint16_t Last_Speed;

    Moto_1_Speed(Speed);
    Last_Speed = Speed;

    return Last_Speed;
}

uint16_t R_MotoState(uint16_t Speed, uint8_t Dir)
{
    uint16_t Last_Speed;

    Moto_2_Speed(Speed);
    Last_Speed = Speed;

    return Last_Speed;
}
