#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "UART.h"
#include "I2C.h"
#include "AD5934.h"

uint8_t data=0;
int main(void){

	DDRB|=(1<<PB0);
	PORTB|=(1<<PB0);
	UART uart;
	sei();
	uart.transmit("hi");
	while(1){
		_delay_ms(50);
		PORTB&=~(1<<PB0);
		_delay_ms(450);
		PORTB|=(1<<PB0);
		if(data){
			data=0;
		}
	}
	return 0;
}/*
ISR(USART_RX_vect){
	data=uart.receive();
}
*/
