#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "Led.h"


int main(void){
	DDRB|=(1<<PB0);
	uart_init();
	initLed();
	sei();
	uart_transmit("\r\n\r\nr/g/b0-255 to set led\r\nv to verbose\r\na to get status\r\n? to randomize\r\nd1-255 to set delay\r\nc1-4 to cycle n bits\r\n");
	setLed(128,0,0);
	for(;;){
		cycle();
	}
}

ISR(USART1_RX_vect){
	uart_isr_rx();
}
