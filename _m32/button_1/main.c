#define nbButton 1
#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>

#include "Button.h"


int main(void){
	
	setConfidenceLevel(0,100);
	DDRB |= (1<<PIN2);	// set led pin as output
	DDRB &= ~(1<<PIN0);	// set button pin as input
	PORTB &=~(1<<PIN2);	// set putton pull-up
	PORTB |= (1<<PIN0);	// set led off
	
	
	while(1){
	if(ButtonPressed(0,PINB,0)){
		PORTB ^= (1<<PIN2);
	}
	}
}