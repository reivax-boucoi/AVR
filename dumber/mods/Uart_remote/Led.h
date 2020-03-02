#ifndef LED_H
#define LED_H

#include "stm32f10x_gpio.h"
#include "stm32f10x.h"

void INIT_LED(void);
void LEDorange(void);
void LEDred(void);
void LEDgreen(void);
void LEDoff(void);


#endif
