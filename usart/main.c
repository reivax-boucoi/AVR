#define F_CPU 12000000UL

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

unsigned char uart_recieve (void){
    while(!(UCSRA) & (1<<RXC));	// wait while data is being received
    return UDR;	// return 8-bit data
}

volatile uint8_t cnt=0;
int main(void){
  DDRB |= 1 << PINB0 ;
  PORTB |= 1<<PINB0;
  uart_init();
  TIMSK|=1<<TOIE0;
  sei();
  TCCR0 |=(1<<CS02) |(1<<CS00);
  
  while(1){
	uint8_t data = uart_recieve ();
	if(data){
		uart_transmit(data);
		data=0;
		}
  }
  return 0;
}
ISR(TIMER0_OVF_vect){
	if(cnt>=46){
		uart_transmit (0x41);
		uart_transmit ('\n');
		uart_transmit (0x42);
		uart_transmit (0x0A);
		PORTB ^= 1<<PINB0 ;
		cnt=0;
	}else{
		cnt++;
	}
}