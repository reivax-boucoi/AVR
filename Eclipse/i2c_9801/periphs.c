#include "periphs.h"


void uart_init (void){
	UBRRH = (BAUDRATE>>8);
	UBRRL = BAUDRATE;
	UCSRB|= (1<<TXEN)|(1<<RXEN)|(1<<RXCIE);
	UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
}

void uart_transmit(char data){
	while (!( UCSRA & (1<<UDRE)));
	UDR = data;
}
void uart_transmitNow(char *data){
	while(*data>0){
		uart_transmit(*data++);
	}
}
unsigned char uart_recieve (void){
	while(!((UCSRA) & (1<<RXC)));
	return UDR;
}
uint8_t uart_cnt = 0;

void uart_isr_rx(void){
}
void uart_isr_udre(void){
}

void i2c_init(void){
	TWBR = (uint8_t)TWBR_val;
}
uint8_t i2c_start(uint8_t address){
	TWCR = 0;
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	while( !(TWCR & (1<<TWINT)) );
	if((TWSR & 0xF8) != TW_START) return 1;
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while( !(TWCR & (1<<TWINT)) );
	uint8_t twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;
	return 0;
}
uint8_t i2c_write(uint8_t data){
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while( !(TWCR & (1<<TWINT)) );
	if( (TWSR & 0xF8) != TW_MT_DATA_ACK ) return 1;
	return 0;
}
uint8_t i2c_read_ack(void){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while( !(TWCR & (1<<TWINT)) );
	return TWDR;
}
uint8_t i2c_read_nack(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while( !(TWCR & (1<<TWINT)) );
	return TWDR;
}
void i2c_stop(void){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

volatile uint16_t tempo = 250;
void adc_init(){
	ADMUX|=(1<<ADLAR);
	ADCSRA|=(1<<ADEN)|(1<<ADSC)|(1<<ADATE)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
uint8_t adc_read(void){
	tempo = (ADCH<<2)+2;
	return tempo;
}
