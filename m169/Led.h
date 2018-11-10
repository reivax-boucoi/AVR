#ifndef LED_H
#define LED_H
#include "defines.h"
#include <avr/io.h>

struct Tcolor{
    uint8_t r,g,b;
}currentColor;
typedef struct Tcolor Tcolor;

Tcolor tcolor(uint8_t r,uint8_t g,uint8_t b);

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
uint8_t ledIsOff(Led l);
uint8_t ledIsOn(Led l);

#endif
