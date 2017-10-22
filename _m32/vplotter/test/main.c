#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD 4800                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

#define XPOS PORTB1
#define XNEG PORTB0
#define YPOS PORTB2
#define YNEG PORTB3
#define XA PINA1
#define XB PORTA2
#define YA PORTA3
#define YB PORTA4

volatile uint8_t test=0;
struct axis {
	uint8_t stop,forward,reverse,state;
	int16_t current;
	int16_t target;
} xaxis,yaxis;
void uart_init (void){
    UBRRH = (BAUDRATE>>8);
    UBRRL = BAUDRATE;	// set baud rate
    UCSRB|= (1<<TXEN)|(1<<RXEN);	// enable receiver and transmitter
    UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);	// 8bit data format
    UCSRB |= (1 << RXCIE ); // Enable the USART Recieve Complete interrupt
}
void ADC_init(void){
	ADMUX =0;//sets right adjusted, AREF referenced, ADC0 measurement channel
	DDRA &=~(1<<PINA0);
	ADCSRA |=(1<<ADEN)|(1<<ADIE)|(1<<ADATE); 
	SFIOR|=(1<<ADTS2);	//set adc to run on timer0 ovf
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
void   axis_init(){
	DDRB |=(1<<XNEG)|(1<<XNEG)|(1<<YPOS)|(1<<YNEG);
	DDRC = 0x00;
	PORTC = 0xFF;
	xaxis.stop = (0<<XNEG)|(0<<XPOS);
	xaxis.forward = (1<<XPOS)|(0<<XNEG);
	xaxis.reverse = (0<<XPOS)|(1<<XNEG);
	//test = PORTA & XA;
	xaxis.current=0;
	xaxis.state=xaxis.stop;
	PORTB = xaxis.state | yaxis.state;
}

volatile uint8_t cnt=0;
int main(void){
  uart_init();
  axis_init();
  //ADC_init();
  TIMSK|=1<<TOIE0;
  sei();
  TCCR0 |=(1<<CS02)|(0<<CS01)|(1<<CS00);
  while(1){
  }
  return 0;
}
ISR(TIMER0_OVF_vect){
	PORTB = xaxis.state | yaxis.state;
	//test = PORTA & XA;
	uart_transmitMult("b\n");
}

ISR ( USART_RXC_vect ){
	
}
/*
ISR(ADC_vect){
	test=0;
	char str[10];
	uint16_t data = ADCL;
	data+= (ADCH<<8);
	PORTB = data>>2;
	itoa((data*15), str, 10);
	uart_transmitMult(str);
	uart_transmit(0x0A);
}*/