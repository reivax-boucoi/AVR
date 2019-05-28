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
	if(t.date==31){
		t.date=1;
	}else if(t.date==0){
		t.date=31;
	}
	clock.setTime(t);
	return t.date;
}
uint16_t MTimeMonth(int8_t a) {
	Time_T t=clock.getTime();
	t.month+=a;
	if(t.month==13){
		t.month=1;
	}else if(t.month==0){
		t.month=12;
	}
	clock.setTime(t);
	return t.month;
}
uint16_t MTimeYear(int8_t a) {
	Time_T t=clock.getTime();
	t.year+=a;
	if(t.year<2000){
		t.year=2000;
	}
	clock.setTime(t);
	return t.year;
}

