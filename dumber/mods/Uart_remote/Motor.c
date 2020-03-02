#include "Motor.h"


	TIM_TimeBaseInitTypeDef TIM2_TempsPWMsettings;
	TIM_OCInitTypeDef TIM2_Configure;

	void MAP_MotorPin(void)	{
		GPIO_InitTypeDef Init_Structure;
	
	  /// Configure les PIN A1 et A2 en output / alternate fonction
		Init_Structure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
		Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
		//Init_Structure.GPIO_Mode = GPIO_Mode_AF_PP;
		Init_Structure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &Init_Structure);
		
		/// Configure les PIN B12,B13,B14, et B15 en output ppull.
		Init_Structure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
		Init_Structure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &Init_Structure);		/// Configure les PIN A12 en output ppull - enable encodeurs
		Init_Structure.GPIO_Pin = GPIO_Pin_12;
		Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
		Init_Structure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &Init_Structure);
		
		/// Configure les PIN A8,A9,A10, et A11 en input floating.
		Init_Structure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
		Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
		Init_Structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &Init_Structure);
		
	}
	

void INIT_TIM2(void){
	TIM2_TempsPWMsettings.TIM_Period = 255;
	TIM2_TempsPWMsettings.TIM_Prescaler = 0;
	TIM2_TempsPWMsettings.TIM_ClockDivision=0;
	TIM2_TempsPWMsettings.TIM_CounterMode=TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &TIM2_TempsPWMsettings);
}

void INIT_OCMotorPwm(void){
	TIM2_Configure.TIM_OCMode=TIM_OCMode_PWM2; 
	TIM2_Configure.TIM_OutputState = TIM_OutputState_Enable;
	TIM2_Configure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM2_Configure.TIM_Pulse = 256;
	TIM2_Configure.TIM_OCPolarity = TIM_OCPolarity_High;
		
	TIM_OC3Init(TIM2, &TIM2_Configure);
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
	TIM_OC2Init(TIM2, &TIM2_Configure);
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	
	TIM_Cmd(TIM2, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM2,ENABLE);
}


void cmdRightMotor(char mod, uint16_t pwm)
{
	pwm = 256-pwm;
	switch (mod)
	{
		case BRAKE :	GPIO_SetBits(GPIOB, GPIO_Pin_12);
									GPIO_SetBits(GPIOB, GPIO_Pin_13);
								break;
		case FORWARD :	GPIO_SetBits(GPIOB, GPIO_Pin_12);
										GPIO_ResetBits(GPIOB, GPIO_Pin_13);
								break;
		
		case REVERSE : GPIO_SetBits(GPIOB, GPIO_Pin_13);
									GPIO_ResetBits(GPIOB, GPIO_Pin_12);
								break;
		
		default : 		GPIO_ResetBits(GPIOB, 12);
									GPIO_ResetBits(GPIOB, 13);
	}
		TIM_SetCompare3(TIM2, pwm);
}





void cmdLeftMotor(char mod, uint16_t pwm)
{    
	pwm = 256-pwm;
	switch (mod)
	{
		case BRAKE : 	GPIO_SetBits(GPIOB, GPIO_Pin_14);
									GPIO_SetBits(GPIOB, GPIO_Pin_15);
								break;
		case FORWARD :  GPIO_SetBits(GPIOB, GPIO_Pin_15);
										GPIO_ResetBits(GPIOB, GPIO_Pin_14);
								break;
		
		case REVERSE : GPIO_SetBits(GPIOB, GPIO_Pin_14);
									 GPIO_ResetBits(GPIOB, GPIO_Pin_15);
								break;
		
		default :GPIO_ResetBits(GPIOB, GPIO_Pin_14);
						 GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	}
	TIM_SetCompare2(TIM2, pwm);
}


void majVitesseMotorG(uint16_t pwm){
		pwm = 256-pwm;
		TIM_SetCompare2(TIM2, pwm);
}

void majVitesseMotorD(uint16_t pwm){
		pwm = 256-pwm;
		TIM_SetCompare3(TIM2, pwm);
}


	
void IC_TIM1_CHANEL1(void)
{
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_ICInit(TIM1, &TIM_ICInitStructure);
	TIM_Cmd(TIM1, ENABLE);
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
}


void IC_TIM1_CHANEL3(void)
{
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_ICInit(TIM1, &TIM_ICInitStructure);
	TIM_Cmd(TIM1, ENABLE);
	TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);
}


void IT_TIM1(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void regulationMoteur(char modRight, char modLeft, uint16_t lapsRight, uint16_t lapsLeft, uint16_t speedRight, uint16_t speedLeft)
{
	cmdRightMotor(modRight,speedRight);
	cmdLeftMotor(modLeft,speedLeft);
	if(modRight==FORWARD){
		tourD=-lapsRight;
	}else if(modRight==REVERSE){
		tourD=lapsRight;
	}
	if(modLeft==FORWARD){
		tourG=-lapsLeft;
	}else if(modLeft==REVERSE){
		tourG=lapsLeft;
	}
}
	

 /*
	* Incremente le nombre de passage devant les capteurs à chaque interuption de ceux ci.
	*/
void TIM1_CC_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TIM1, TIM_IT_CC1) == SET) 
  {
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
		tourD++;
		AbsPosRight++;
		if(tourD==0){
			cmdRightMotor(BRAKE,255);
		}
  }
	
  if(TIM_GetITStatus(TIM1, TIM_IT_CC3) == SET) 
  {
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
		tourG++;
		AbsPosLeft++;
		if(tourG==0){
			cmdLeftMotor(BRAKE,255);
		}
  }
}
