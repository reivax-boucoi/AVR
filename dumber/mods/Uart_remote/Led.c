#include "Led.h"

void INIT_LED(void){
	GPIO_InitTypeDef gpioInit;
	gpioInit.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &gpioInit);
}
void LEDorange(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
}

void LEDred(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
}

void LEDgreen(void)
{
	
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
}

void LEDoff(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}
