#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>


int main(void){
	DDRB |= (1 << PB0);
	PORTB |= (1<<PB0);
	LCDCRA|=(1<<LCDEN);
	LCDCRB|=(1<<LCDMUX1)|(1<<LCDMUX0);
	LCDFRR|=(1<<LCDPS2)|(1<<LCDCD1)|(1<<LCDCD0); // prescaler 512, division factor 4 & K=8 for duty 1/4
	while(1){
		PORTB^=(1<<PB0);
		_delay_ms(500);
	}
	return 0;
}
