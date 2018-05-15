#include "avr/io.h"
#include "avr/interrupt.h"
#include "uart_interpreter.h"
#include "cmds.h"

int main(void){
	uart_init();
	DDRC|=0xFC;
	sei();
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
