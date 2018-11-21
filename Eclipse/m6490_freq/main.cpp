#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h>
//#include "include/LCD.h"

//int8_t i=1;
//LCD myLCD;
int main(void){
	DDRB|=(1<<PB0);
	DDRB&=~(1<<PB4);
	PORTB|=(1<<PB4);
	//myLCD.clear();
	sei();
	while(1){
		if(PINB&PB4){
			PORTB|=(1<<PB0);
		}else{
			PORTB&=~(1<<PB0);
		}
	}
	return 1;
}

