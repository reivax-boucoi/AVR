#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 4800                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR
#define CS PINB2
// ADC config:xxxxSSx1 0
#define VCH0 0b00001100
#define ICH0 0b00001001 // CH2ref-/3sig+ (diff)

void uart_init (void){
    UBRR0H = (BAUDRATE>>8);
    UBRR0L = BAUDRATE;	// set baud rate
    UCSR0B|= (1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);	// enable receiver and transmitter
    UCSR0C|= (1<<UCSZ00)|(1<<UCSZ01);	// 8bit data format
}
void uart_transmit (uint8_t data){
    while (!( UCSR0A & (1<<UDRE0)));	// wait while register is free
    UDR0 = data;	// load data in the register
}
void uart_transmitMult(char *Data){
	while(*Data>0){
		uart_transmit(*Data++);
	}
}
uint8_t uart_recieve (void){
    while(!(UCSR0A) & (1<<RXC0));	// wait while data is being received
    return UDR0;	// return 8-bit data
}

void spi_masterInit(void){
DDRB = (1<<PINB3)|(1<<PINB5)|(1<<CS);
SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)/*|(1<<SPIE)*/; //sample rising & setup falling
PORTB |=(1<<CS);
}
uint8_t spi_rxtx(uint8_t data){
SPDR = data;
while(!(SPSR & (1<<SPIF)));
return (SPDR);
}
/*
uint16_t adc_v(void){
	uint16_t val=0;
	PORTB &=~(1<<CS);
	spi_rxtx(VCH0);
	val=(spi_rxtx(128) &15)<<8;
	val|=spi_rxtx(0);
	PORTB |=(1<<CS);
	return (val);
}

int16_t adc_i(void){
	uint16_t val=0;
	PORTB &=~(1<<CS);
	spi_rxtx(ICH0);
	uint8_t sign=spi_rxtx(128);;
	val=(sign&15)<<8;
	sign=(sign&0b00010000);	
	val|=spi_rxtx(0);;
	PORTB |=(1<<CS);
	if(sign)return (-val);
	return (val);
}

*/
volatile uint8_t data;
volatile uint8_t cnt=0;
volatile int64_t acc=0;

int main(void){
	DDRD |=(1<<PIND5)|(1<<PIND6);
	DDRB |= 1 << PINB0 ;
	PORTB |= 1<<PINB0;
	uart_init();
	uart_transmitMult("\ninitialized uart !\n");
	spi_masterInit();
	uart_transmitMult("initialized spi !\n");
	TCCR0A |=(1<<WGM01); // CTC mode
	TIMSK0|=1<<OCIE0A;
	OCR0A=7; // 16MHz/(2*1024*(1+OCR0A))=1.953.125KHz
	sei();
	TCCR0B |=(1<<CS02) |(1<<CS00); // N=1024

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
ISR(TIMER0_COMPA_vect){
	//PORTD |=(1<<PIND6);
	//int32_t p = adc_v()*adc_i();
	//acc+=p;
	cnt++;
	if(cnt>=255){
	//PORTD |=(1<<PIND5);
	/*	char str[10]={0};
		itoa((int)(acc/cnt),str,10);
		uart_transmitMult(str);
		uart_transmit('\n');*/
		cnt=0;
	//	acc=0;
	//PORTD &=~(1<<PIND5);
	}
	//PORTD &=~(1<<PIND6);
}
/*ISR(SPI_STC_vect){
	
}*/