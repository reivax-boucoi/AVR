// Blinks an LED on Pin 8 (bottom left)

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"

	LCD myLCD ;
//using namespace symbols;
int main(void){
	DDRB |= (1 << PINB0) | (1<<PINB1);
	PORTB |= (1<<PINB0) | (0<PINB1);
	sei();
	while(1){
		myLCD.Blink();
		_delay_ms(250);

	}
	return 0;
}
ISR(TIMER0_COMP_vect){
	myLCD.timer();
}
