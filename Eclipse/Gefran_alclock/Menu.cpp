#include "Menu.h"



uint16_t test(int8_t a){
	return a+1;
}

uint16_t MTimeHour(int8_t a) {
	Time_T t=clock.getTime();
	t.hour+=a;
	if(t.hour>23)t.hour=0;
	if(t.hour<0)t.hour=23;
	clock.setTime(t);
	return t.hour;
}
uint16_t MTimeMin(int8_t a) {
	Time_T t=clock.getTime();
	t.min+=a;
	if(t.min>59)t.min=0;
	if(t.min<0)t.min=59;
	clock.setTime(t);
	return t.min;
}

uint16_t MTimeDate(int8_t a) {
	Time_T t=clock.getTime();
	t.date+=a;
	if(t.date>59)t.date=0;
	if(t.date<0)t.date=59;
	clock.setTime(t);
	return t.date;
}
