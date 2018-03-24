#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"

int main(void){
	DDRB |= (1 << PB0);
	PORTB |= (1<<PB0);
	int8_t i=0;
	while(1){
		_delay_ms(128);

	}
	return 0;
}
