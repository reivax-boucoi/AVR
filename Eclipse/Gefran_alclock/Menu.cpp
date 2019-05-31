#include "Menu.h"



uint16_t test(int8_t a){
	return a+1;
}

uint16_t MTimeHour(int8_t a) {
	Time_T t=clock.getTime();
	t.hour+=a;
	if(t.hour>59)t.hour=0;
	if(t.hour<0)t.hour=59;
	clock.setTime(t);
	return t.hour;
}
