#ifndef MENU_H_
#define MENU_H_
#include <avr/io.h>
//#include "Clock.h"

uint16_t test(uint8_t a);
uint16_t MTimeHour(uint8_t a);


class Menu{
public:
	char name[5];
	uint16_t (*fptr)(uint8_t);
	Menu* parent;
	Menu** subs;
	uint8_t nbSubs;
};


#endif /* MENU_H_ */
