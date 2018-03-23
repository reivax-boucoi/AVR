#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
LCD myLCD;
#define H 'H'
int main(void){
	DDRB |= (1 << PB0);
	PORTB |= (1<<PB0);
	while(1){
		myLCD.setNb(H);
		_delay_ms(128);
		myLCD.blink(1);

	}
	return 0;
}
