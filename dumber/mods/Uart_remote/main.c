#include <stm32f10x.h>
#include "stm32f10x_rcc.h"
#include "Led.h"
#include "Motor.h"
#include "Uart.h"

int16_t tourG, tourD, AbsPosRight,AbsPosLeft;

void delay(int d){
	for(int i=0;i<d;i++){for(int j=0;j<1000;j++);}
}
int main(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	
	INIT_LED();
	LEDred();
	
	MAP_MotorPin();
	INIT_TIM2();
	INIT_OCMotorPwm();
	IC_TIM1_CHANEL3();
	IC_TIM1_CHANEL1();
	IT_TIM1();
	
	MAP_UsartPin();
	INIT_USART();
	INIT_IT_UsartReceive();
	
	//regulationMoteur(FORWARD,FORWARD,1000,500,255,128);
	
	LEDoff();
	
	
	while(1){
		delay(500);
		LEDorange();
		delay(500);
		LEDoff();
		USART_SendData(USART1,'A');
	}
}
