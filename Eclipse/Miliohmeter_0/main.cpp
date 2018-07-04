#include <avr/io.h>
#include <util/delay.h>
#include "include/cmds.h"
#include "include/uart_interpreter.h"
#include "include/LCD.h"

LCD myLCD;
float LCDval=0.0;

int main(void){
	uart_init();
	myLCD.blink();
	_delay_ms(1000);
	sei();
	uart_transmitnl("\tHello World !");
	uart_prompt();
	while(1){
		LCDval+=.01;
		_delay_ms(100);
		myLCD.setNb(LCDval);

	}
	return 0;
}

ISR(USART0_RX_vect){
	uart_isr_rxc();
}

ISR(USART0_UDRE_vect){
	uart_isr_udre();
}

