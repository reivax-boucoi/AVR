#include "avr/io.h"
#include "avr/interrupt.h"
#include "periphs.h"

int main(void){
	uart_init();
	sei();

	for(;;) {
		uart_transmitByte(uart_receiveByte());
	}
	return 0;
}

ISR(USART_RXC_vect){
	uart_isr_rxc();
}

ISR(USART_UDRE_vect){
	uart_isr_udre();
}
