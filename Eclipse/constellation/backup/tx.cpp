#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"


int main(void){
	DDRB|=(1<<PB0);
	uart_init();
	sei();
	for(;;){
		PORTB^=(1<<PB0);
		uart_transmit("Hello world\r\n");
		_delay_ms(500);
	}
}

ISR(USART1_RX_vect){
	uart_isr_rx();
}
