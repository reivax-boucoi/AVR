#ifndef RTC_H
#define RTC_H
#include "defines.h"
#include <avr/io.h>

struct Ttime {
	uint8_t hour, min, monthDay, month;
    int8_t temp;
}currentTime;
typedef struct Ttime Ttime;

void setCurrentTime(uint8_t hour,uint8_t min,uint8_t monthDay,uint8_t month);
uint8_t minquad(uint8_t min);
uint8_t bcdToDec(uint8_t val);
#endif
