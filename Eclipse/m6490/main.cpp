#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "include/LCD.h"

int8_t i=1;
LCD myLCD;
int main(void){
	DDRB|=(1<<PB0);
	//myLCD.clear();

	while(1){
		PORTB^=(1<<PB0);
		_delay_ms(500);
	}
	return 1;
}


