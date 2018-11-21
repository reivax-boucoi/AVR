#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
int main(void){
	DDRB|=(1<<PB0)|(1<<PB4);
	for(;;){
		PORTB^=(1<<PB0)|(1<<PB4);
		_delay_ms();
	}
	return 0;
}


