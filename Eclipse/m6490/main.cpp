<<<<<<< HEAD
#include <avr/io.h>
#include <util/delay.h>

//float LCDval=0.0;
int main(void){
	DDRF|=(1<<PF0);
	LCDCRB|=(1<<LCDMUX0)|(1<<LCDMUX1)|(1<<LCDPM3)|(1<<LCDPM2)|(1<<LCDPM1)|(1<<LCDPM0);
	LCDFRR|=(1<<LCDPS0)|(1<<LCDPS1)|(1<<LCDPS2)|(1<<LCDCD2);	//8MHz/(4096*8*5)=48.8Hz
	LCDCCR|=(1<<LCDCC3);
	LCDCRA|=(1<<LCDEN);
	//LCDCRA|=(1<<LCDBL);
	while(1){
		PORTF^=(1<<PF0);
		LCDDR00=0xFF;
		LCDDR01=0xFF;
		LCDDR02=0xFF;
		LCDDR03=0xFF;
		LCDDR04=0xFF;
		LCDDR05=0xFF;
		LCDDR06=0xFF;
		LCDDR07=0xFF;
		LCDDR08=0xFF;
		LCDDR09=0xFF;
		LCDDR10=0xFF;
		LCDDR11=0xFF;
		LCDDR12=0xFF;
		LCDDR13=0xFF;
		LCDDR14=0xFF;
		LCDDR15=0xFF;
		LCDDR16=0xFF;
		LCDDR17=0xFF;
		LCDDR18=0xFF;
		LCDDR19=0xFF;
		_delay_ms(500);
		LCDDR00=0x00;
		LCDDR01=0x00;
		LCDDR02=0x00;
		LCDDR03=0x00;
		LCDDR04=0x00;
		LCDDR05=0x00;
		LCDDR06=0x00;
		LCDDR07=0x00;
		LCDDR08=0x00;
		LCDDR09=0x00;
		LCDDR10=0x00;
		LCDDR11=0x00;
		LCDDR12=0x00;
		LCDDR13=0x00;
		LCDDR14=0x00;
		LCDDR15=0x00;
		LCDDR16=0x00;
		LCDDR17=0x00;
		LCDDR18=0x00;
		LCDDR19=0x00;
		_delay_ms(500);
	}
	return 1;
}

=======
#include <avr/io.h>
#include <avr/interrupt.h>
#include "include/uart_interpreter.h"

float LCDval=0.0;
int main(void){
	uart_init();
	DDRB|=(1<<PB6)|(1<<PB7)|(1<<PB5);
	TCCR1A|=(1<<COM1A0)|(1<<COM1A1);
	TCCR1B|=(1<<WGM12);
	OCR1A=0x00FF;
	TIMSK1|=(1<<TOIE1);
	sei();
	TCCR1B|=(1<<CS10);
	uart_transmit("\r\n\tHello World !");
	uart_prompt();
	while(1){
	}
	return 1;
}

ISR(USART_RX_vect){
	uart_isr_rxc();
}
ISR(USART_UDRE_vect){
	uart_isr_udre();
}
>>>>>>> 9d3b0f646835cac570ebc3fe71356051f60b58ad
