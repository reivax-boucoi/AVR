#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
/*
#define BAUD 38400
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

void uart_init (void);
void uart_transmit (unsigned char data);
unsigned char uart_recieve (void);
void uart_transmitMult(char *Data);*/
int main(void){
	//uart_init();
	//sei();
	DDRB &=~(1<<PB0);
	DDRD|=(1<<PD7);
	PORTD &=~(1<<PD7);
	//uart_transmitMult("test\r\n");
	while(1){
		if(PINB&(1<<PB0)){
			PORTD|=(1<<PD7);
		}else{
			PORTD&=~(1<<PD7);
		}
		_delay_ms(2);

	}
	return 0;
}/*
ISR ( USART_RXC_vect ){
	char ReceivedByte ;
	ReceivedByte = UDR ;
	UDR = ReceivedByte ;
}

void uart_init (void){
   UBRRH = (BAUDRATE>>8);
   UBRRL = BAUDRATE;
   UCSRB|= (1<<TXEN)|(1<<RXEN);
   UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
   UCSRB |= (1 << RXCIE );
}
void uart_transmit (unsigned char data){
   while (!( UCSRA & (1<<UDRE)));
   UDR = data;
}
unsigned char uart_recieve (void){
   while(!(UCSRA) & (1<<RXC));
   return UDR;
}
void uart_transmitMult(char *Data){
	while(*Data>0){
		uart_transmit(*Data++);
	}
}
*/
