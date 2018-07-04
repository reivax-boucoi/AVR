#include <avr/io.h>
#include "include/cmds.h"
#include "include/uart_interpreter.h"


int main(void){
	uart_init();
	sei();
	uart_transmitnl("\tHello World !");
	uart_prompt();
	while(1){

	}
	return 0;
}

ISR(USART0_RX_vect){
	uart_isr_rxc();
}

ISR(USART0_UDRE_vect){
	uart_isr_udre();
}

