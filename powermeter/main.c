#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 2400                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR


  void uart_init (void){
    UBRR0H = (BAUDRATE>>8);
    UBRR0L = BAUDRATE;	// set baud rate
    UCSR0B|= (1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);	// enable receiver and transmitter
    UCSR0C|= (1<<UCSZ00)|(1<<UCSZ01);	// 8bit data format
}

void uart_transmit (unsigned char data){
    while (!( UCSR0A & (1<<UDRE0)));	// wait while register is free
    UDR0 = data;	// load data in the register
}

unsigned char uart_recieve (void){
    while(!(UCSR0A) & (1<<RXC0));	// wait while data is being received
    return UDR0;	// return 8-bit data
}

volatile uint8_t data;
volatile uint8_t cnt=0;

int main(void){
	DDRD |=(1<<PIND5);
	DDRB |= 1 << PINB0 ;
	PORTB |= 1<<PINB0;
	uart_init();
	TIMSK0|=1<<TOIE0;
	sei();
	TCCR0B |=(1<<CS02) |(1<<CS00);

	while(1){
		if(data>0){
			uart_transmit(data);
			if(data=='a')PORTD ^=(1<<PIND5);
			data=0;
		}
	}
	return 0;
}
ISR(USART_RX_vect, ISR_BLOCK){
	data=uart_recieve();
}
ISR(TIMER0_OVF_vect){
	cnt++;
	if(cnt>100){
		//PORTD ^=(1<<PIND5);
		uart_transmit('b');
		cnt=0;
	}
}

