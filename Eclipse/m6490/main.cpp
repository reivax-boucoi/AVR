#include <avr/io.h>
#include <util/delay.h>
#include "include/LCD.h"

LCD myLCD;
int main(void){
	DDRF|=(1<<PF0);
	myLCD.setBattery(LCD::FULL);
	myLCD.setNb(9647,DSMALL);
	myLCD.setNb(-1234567,DBIG);
	while(1){
		PORTF^=(1<<PF0);
		_delay_ms(500);
	}
	return 1;
}

