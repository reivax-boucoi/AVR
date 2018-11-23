#ifndef LED_H_
#define LED_H_
#include <avr/io.h>
#include "uart.h"

#define LEDR PA5
#define LEDG PA6
#define LEDB PB3
#define OCRR OCR0B
#define OCRG OCR1BL
#define OCRB OCR1AL

void setLed(uint8_t rgb, uint8_t val);
void setLed(uint8_t r, uint8_t g, uint8_t b);
void initLed(void);
void printLed(void);
void randomize(void);
void cycle(void);
void setMode(char b);
void setMode(char b,uint8_t r);

#endif /* LED_H_ */
