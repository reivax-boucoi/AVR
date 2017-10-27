#define F_CPU 8000000UL
# include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD 4800                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

void uart_init (void){
    UBRRH = (BAUDRATE>>8);
    UBRRL = BAUDRATE;	// set baud rate
    UCSRB|= (1<<TXEN)|(1<<RXEN);	// enable receiver and transmitter
    UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);	// 8bit data format
    UCSRB |= (1 << RXCIE ); // Enable the USART Recieve Complete interrupt
}

void uart_transmit(unsigned char data){
    while (!( UCSRA & (1<<UDRE)));	// wait while register is free
    UDR = data;	// load data in the register
}

void uart_transmitMult(char *Data){
	while(*Data>0){
		uart_transmit(*Data++);
	}
}

unsigned char uart_recieve (void){
    while(!(UCSRA) & (1<<RXC));	// wait while data is being received
    return UDR;	// return 8-bit data
}

int main (void){
	uart_init();
	TIMSK|=1<<TOIE0;
	sei();
	TCCR0 |=(1<<CS02)|(0<<CS01)|(1<<CS00);
    DDRB = 0xFF;
	PORTB = 0x00;
    DDRA &= ~(1 << PA0); 

    while (1){
	}
    return 0;
}

ISR(TIMER0_OVF_vect){
	if (PINA & (1<<PINA0))uart_transmitMult("hi\n");
}

ISR ( USART_RXC_vect ){
	
}