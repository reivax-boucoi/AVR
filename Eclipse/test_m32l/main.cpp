// Blinks an LED on Pin 8 (bottom left)

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
//#include "LCD.h"

//using namespace symbols;
int main(void){
	//LCD myLCD ;
	DDRB |= (1 << PINB0) | (1<<PINB1);
	PORTB |= (1<<PINB0) | (0<PINB1);

	while(1){

		_delay_ms(250);

	}
	return 0;
}
