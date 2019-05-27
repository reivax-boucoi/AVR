#ifndef MENU_H_
#define MENU_H_
#include <avr/io.h>
#include "Clock.h"

uint16_t test(int8_t a);


uint16_t MTimeHour(int8_t a);
uint16_t MTimeMin(int8_t a);
uint16_t MTimeDate(int8_t a);
uint16_t MTimeMonth(int8_t a);
uint16_t MTimeYear(int8_t a);

uint16_t MAlarmHour(int8_t a);
uint16_t MAlarmMin(int8_t a);
uint16_t MAlarmActivate(int8_t a);


class Menu{
public:
	char name[5];
	uint16_t (*fptr)(int8_t);
	Menu* parent;
	Menu** subs;
	uint8_t nbSubs;
};


#endif /* MENU_H_ */
