#include <stdio.h>
#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <avr/interrupt.h>
#include "periphs.h"


int main(void){
	DDRB |= (1 << PB0) | (1<<PB1);
	PORTB |= (1<<PB0) | (0<PB1);
	DDRC |=(1<<PA7);
	uart_init();
	adc_init();
	sei();
	float temp=1.234;
	while(1){
		 temp +=1;
		if(temp>22.0)PORTC^=(1<<PA7);
		char str[10] = {0};
		sprintf(str, "T = %i\r\n",tempo);
		uart_transmitBuff(str,sizeof(str));
		PORTB^=(1<<PB0);
		_delay_ms(tempo);
	}
	return 0;
}
ISR (ADC_vect ){
	adc_read();
}/*
ISR (USART_RXC_vect ){
	uart_isr_rx();
}*/
ISR (USART_UDRE_vect ){
	uart_isr_udre();
}
