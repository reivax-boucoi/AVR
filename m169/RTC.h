#ifndef RTC_H
#define RTC_H
#include "defines.h"
#include <avr/io.h>

struct Ttime {
	uint8_t hour, min, minquad, monthDay, month;
}currentTime;
typedef struct Ttime Ttime;

void setCurrentTime(uint8_t hour,uint8_t min,uint8_t minquad,uint8_t monthDay,uint8_t month);
#endif
