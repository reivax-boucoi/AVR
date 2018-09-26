#include "periphs.h"


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
	uint8_t sign=spi_rxtx(128);
	val=(sign&15)<<8;
	sign=(sign&0b00010000);
	val|=spi_rxtx(0);
	PORTB |=(1<<CS);
	if(sign)return (val-4096);
	return (val);
}
