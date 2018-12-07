#ifndef LED_H
#define LED_H
#include "defines.h"
#include "RTC.h"
#include <avr/io.h>

struct Tcolor{
    uint8_t r,g,b;
}currentColor;
typedef struct Tcolor Tcolor;

Tcolor tcolor(uint8_t r,uint8_t g,uint8_t b);
Tcolor tcolorV(uint32_t v);

struct Led{
    uint8_t col;//0->9
    uint8_t row;//0 or 1
    Tcolor c;
};
typedef struct Led Led;

void ledInit(Led* leds);
uint32_t getColorByLed(Led l);
uint32_t getDataByColor(Tcolor c, uint8_t mode, Led* leds);
void ledOff(Led* l);
void ledOn(Led* l);
void ledOnC(Led* l,Tcolor c);
void ledOnV(Led* l,uint32_t v);

void setLeds(Ttime t, Led* l, Tcolor c);
void setLedsNb(int8_t nb, Led* l, Tcolor c);


void sendRawData(uint32_t data);

#endif
