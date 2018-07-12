#define F_CPU 1000000UL

#include <avr/io.h>
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
volatile uint8_t test=0;
void ADC_init(void){
	ADMUX =0;//sets right adjusted, AREF referenced, ADC0 measurement channel
	DDRA &=~(1<<PINA0);
	ADCSRA |=(1<<ADEN)|(1<<ADIE)|(1<<ADATE); 
	SFIOR|=(1<<ADTS2);	//set adc to run on timer0 ovf
}
void DAC_init(void){
	DDRB |= 0b11111111;	
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

volatile uint8_t cnt=0;
int main(void){
  uart_init();
  DAC_init();
  ADC_init();
  TIMSK|=1<<TOIE0;
  sei();
  TCCR0 |=(1<<CS02)|(0<<CS01)|(0<<CS00);
  
  while(1){

  }
  return 0;
}
ISR(TIMER0_OVF_vect){
	if(test>0){
		uart_transmitMult("help!");
	}
	test=1;			
}

ISR ( USART_RXC_vect ){
	
}

ISR(ADC_vect){
	test=0;
	char str[10];
	uint16_t data = ADCL;
	data+= (ADCH<<8);
	PORTB = data>>2;
	itoa((data*15), str, 10);
	uart_transmitMult(str);
	uart_transmit(0x0A);
}