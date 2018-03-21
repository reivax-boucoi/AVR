#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
LCD myLCD;

int main(void){
	DDRB |= (1 << PB0);
	PORTB |= (1<<PB0);
	myLCD.setDigit(0,0);

	while(1){
		PORTB^=(1<<PB0);
		_delay_ms(500);
	}
	return 0;
}
