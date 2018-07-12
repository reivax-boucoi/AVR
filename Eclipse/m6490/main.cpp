#include <avr/io.h>
#include <avr/interrupt.h>
#include "include/uart_interpreter.h"

float LCDval=0.0;
int main(void){
	uart_init();
	DDRB|=(1<<PB6)|(1<<PB7)|(1<<PB5);
	TCCR1A|=(1<<COM1A0)|(1<<COM1A1);
	TCCR1B|=(1<<WGM12);
	OCR1A=0x00FF;
	TIMSK1|=(1<<TOIE1);
	sei();
	TCCR1B|=(1<<CS10);
	uart_transmit("\r\n\tHello World !");
	uart_prompt();
	while(1){
	}
	return 1;
}

ISR(USART_RX_vect){
	uart_isr_rxc();
}
ISR(USART_UDRE_vect){
	uart_isr_udre();
}
