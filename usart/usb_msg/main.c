#define F_CPU 12000000UL

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

void uart_init (void){
    UBRRH = (BAUDRATE>>8);
    UBRRL = BAUDRATE;	// set baud rate
    UCSRB|= (1<<TXEN)|(1<<RXEN);	// enable receiver and transmitter
    UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);	// 8bit data format
}

void uart_transmit (unsigned char data){
    while (!( UCSRA & (1<<UDRE)));	// wait while register is free
    UDR = data;	// load data in the register
}

void uart_transmitMult(char *Data){
	while(*Data>0){
		uart_transmit(*Data++);
	}
}

volatile uint8_t cnt=0;
volatile uint16_t count=0;

void routine(void){
	char str[10]={0};
	itoa(count,str,10);
	PORTB ^= 1<<PINB0 ;
	count++;
	uart_transmitMult(str);
	uart_transmit('\n');

}

int main(void){
  DDRB |= 1 << PINB0 ;
  DDRD&=~(1<<PIND2);
  PORTB |= 1<<PINB0 ;
  PORTD|= 1<<PIND2;
  
  uart_init();
  
  TIMSK|=1<<TOIE0;
  MCUCR|=1<<ISC01;
  GICR|=1<<INT0;
  sei();
  TCCR0 |=(1<<CS02) |(1<<CS00);
  
  while(1){
  
  }
  
  return 0;
  
}
ISR(INT0_vect){
	routine();
}
ISR(TIMER0_OVF_vect){
	if(cnt>=200){
		routine();
		cnt=0;
	}else{
		cnt++;
	}
}