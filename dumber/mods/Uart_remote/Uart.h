#ifndef UART_H
#define UART_H

#include "stm32f10x.h"
#include "Led.h"


void INIT_IT_UsartReceive(void);
void INIT_USART(void);
void MAP_UsartPin(void);

#endif 

