#include <avr/io.h>
#include <util/delay.h>

int main(void){
	DDRB|=(1<<PB6);
	while(1){
		_delay_ms(500);
		PORTB^=(1<<PB6);
	}
	return 1;
}
