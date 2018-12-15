#define F_CPU 16000000UL
#include "periph.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t data;
int main(void){    
	DDRB|=(1<<PB0);
	PORTB|=(1<<PB0);
  uart_init();
  TIMSK0|=1<<TOIE0;
  sei();
  TCCR0B |=(1<<CS02) |(1<<CS00);
    while(1){
		_delay_ms(500);
		PORTB^=(1<<PB0);
    }
    return(0);
    
}
ISR(TIMER0_OVF_vect){
	uart_transmit("Hello world\r\n");
}