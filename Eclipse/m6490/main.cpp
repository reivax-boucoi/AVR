#include <avr/io.h>
#include <util/delay.h>
#include "include/LCD.h"
//float LCDval=0.0;
uint8_t i=0;
LCD myLcd;
int main(void){
	DDRF|=(1<<PF0);
	myLcd.setNb(1111,DSMALL);
	while(1){
		PORTF^=(1<<PF0);
		_delay_ms(500);
		_delay_ms(500);
	}
	return 1;
}

