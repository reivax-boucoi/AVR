#ifndef LED_H_
#define LED_H_
#include <avr/io.h>
#include "uart.h"

#define LEDR PA6
#define LEDG PA5
#define LEDB PB3

void setLed(uint8_t rgb, uint8_t val);
void setLed(uint8_t r, uint8_t g, uint8_t b);
void initLed(void);
void printLed(void);

#endif /* LED_H_ */
