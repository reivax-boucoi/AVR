#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "Led.h"


int main(void){
	DDRB|=(1<<PB0);
	uart_init();
	initLed();
	sei();
	uart_transmit("Hello world\r\n\r\n>");
	setLed(128,0,0);
	for(;;){
		PORTB^=(1<<PB0);
	}
}

ISR(USART1_RX_vect){
	uart_isr_rx();
}
