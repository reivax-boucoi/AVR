#include <stm32f10x.h>
#include "Uart.h"


/**
	* @brief 			La fonction mapUsartPin va venir configurer le E/S du GPIO pour correspondre avec le schéma electrique en ressource.
	*							PB7 Analog Input / PB6 Alternate function output.
	*
	* @param  		Aucun
	* @retval 		Aucun
	*/
void MAP_UsartPin()
{
	GPIO_InitTypeDef Init_Structure;
	/// Configure Output ALTERNATE FONCTION PPULL PORT B6 Tx
	Init_Structure.GPIO_Pin = GPIO_Pin_6;
	Init_Structure.GPIO_Mode = GPIO_Mode_AF_PP;
	Init_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &Init_Structure);
	
	/// Configure B7 Rx
	Init_Structure.GPIO_Pin = GPIO_Pin_7;
  Init_Structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &Init_Structure);
}

void INIT_IT_UsartReceive(void)
{
	   NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USARTz Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
	* @brief Initialise l'UART avec les paramétres suivants : 9600 bauds / 1bits de stop / pas de partité ou de controle
	*	
	* @param  Aucun
	* @retval Aucun
	*/

void INIT_USART(void)
{
		USART_InitTypeDef USART_InitStructure;
	
		USART_InitStructure.USART_BaudRate = 9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
			
		USART_Init(USART1, &USART_InitStructure);
			
		USART_Cmd(USART1, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}


void USART1_IRQHandler(void){
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE) !=RESET){
		char c = USART_ReceiveData(USART1);
		USART_SendData(USART1,c);
	}
	
	USART_ClearFlag(USART1, USART_FLAG_RXNE);
}


