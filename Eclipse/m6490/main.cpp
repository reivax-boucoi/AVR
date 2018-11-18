#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int8_t i=1;
int main(void){
	DDRB|=(1<<PB0);
	while(1){
		PORTB^=(1<<PB0);
		_delay_ms(500);
	}
	return 1;
}


