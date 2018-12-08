#ifndef LED_H
#define LED_H
#include "defines.h"
#include "RTC.h"
#include <avr/io.h>



struct Led{
    uint8_t col;//0->9
    uint8_t row;//0 or 1
    uint8_t c;//0b00000rgb
};
typedef struct Led Led;

void ledInit(Led* leds);
uint32_t getDataByColor(uint8_t c, uint8_t mode, Led* leds);
void ledOff(Led* l);
void ledOn(Led* l,uint8_t c);

void setLeds(Ttime t, Led* l);
void setLedsNb(int8_t nb, Led* l);


void sendRawData(uint32_t data);

#endif
