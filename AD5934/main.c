#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void){    
	DDRB|=(1<<PB0);
	PORTB|=(1<<PB0);
    while(1){
		_delay_ms(500);
		PORTB^=(1<<PB0);
    }
    return(0);
    
}
