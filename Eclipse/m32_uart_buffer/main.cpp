#include "avr/io.h"
#include "avr/interrupt.h"
#include "periphs.h"
#include "cmds.h"

int main(void){
	uart_init();
	DDRB|=(1<<PB0);
	sei();
	uart_transmit((char*)"Hello World\n");
	uart_transmit((char*)"Hello World\n");
	onLed();
	while(1) {

	}
	return 0;
}

ISR(USART_RXC_vect){
	uart_isr_rxc();
}

ISR(USART_UDRE_vect){
	uart_isr_udre();
}
