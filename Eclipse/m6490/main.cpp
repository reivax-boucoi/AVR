#include <avr/io.h>
#include <util/delay.h>
#include "include/LCD.h"
//float LCDval=0.0;
int8_t i=1;
LCD myLcd;
int main(void){
	DDRF|=(1<<PF0);
	myLcd.setDigit(0,i,DBIG);
	myLcd.setDigit(1,i,DBIG);
	myLcd.setDigit(2,i,DBIG);
	myLcd.setDigit(3,i,DBIG);
	myLcd.setDigit(4,i,DBIG);
	myLcd.setDigit(5,i,DBIG);
	myLcd.setDigit(6,i,DBIG);
	while(1){
		PORTF^=(1<<PF0);
		_delay_ms(50);

	}
	return 1;
}

