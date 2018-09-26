#define F_CPU 12000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR
#define string "HELLO WORLD I AM WRITING A LONG MESSAGE, I HOPE IT DOESN'T TAKE TOO LONG TO ARRIVE ALSO WHY AM I TYPING USING CAPITALS ? THIS IS STARTING TO GET A BIT SILLY NO ? I WONDER I I FILLED MY  MEMORY ALREADY OR IF I SHOULD CONTINUE LOOKS LIKE I SHOULD CONTINUE BECAUSE THER IS STILL 30% LEFT, but I have a feeling it could take a bit longer than expected...DO YOU THINK WE WILL HIT A LIMIT AT A CERTAIN POINT ? BECAUSE I FEEL LIKE IT IS GONNA TAKE FOREVER, WE ARE ALREADY AT 400+ CHARACTERS AND ONLY A THIRD OF THE WAY DONE !!!\n"
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
int main(void){
  DDRB |= 1 << PINB0 ;
  PORTB |= 1<<PINB0;
  uart_init();
  TIMSK|=1<<TOIE0;
  sei();
  TCCR0 |=(1<<CS02) |(1<<CS00);
  
  while(1){
  }
  return 0;
}
ISR(TIMER0_OVF_vect){
	if(cnt>=1){
		uart_transmitMult(string);
		PORTB ^= 1<<PINB0 ;
		cnt=0;
	}else{
		cnt++;
	}
}