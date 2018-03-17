#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#define SLA 0x40
#define TEMPREG 0xE3
#define I2C_READ 0x01
#define I2C_WRITE 0x00

#define F_SCL 50000UL
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16 ) / 2)
void i2c_init(void);
uint8_t i2c_start(uint8_t address);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);
void i2c_stop(void);

#define BAUDRATE 4800UL
#define BAUD (F_CPU/(16UL*BAUDRATE)-1)
void uart_init (void){
  UBRRH = (BAUDRATE>>8);
  UBRRL = BAUDRATE;	// set baud rate
  UCSRB|= (1<<TXEN)|(1<<RXEN);	// enable receiver and transmitter
  UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);	// 8bit data format
  UCSRB |= (1 << RXCIE ); // Enable the USART Recieve Complete interrupt
}

void uart_transmit (unsigned char data){
  while (!( UCSRA & (1<<UDRE)));	// wait while register is free
  UDR = data;	// load data in the register
}

unsigned char uart_recieve (void){
  while(!(UCSRA) & (1<<RXC));	// wait while data is being received
  return UDR;	// return 8-bit data
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
float calc(uint16_t v){
	return ((175.72*v)/65536)-46.85;
}
int main(void){
	DDRB |= (1 << PB0) | (1<<PB1);
	PORTB |= (1<<PB0) | (0<PB1);
	DDRC |=(1<<PA7);
	i2c_init();
	while(1){
		uint16_t data=0;
		i2c_start((SLA<<1)+I2C_WRITE);
		i2c_write(TEMPREG);
		i2c_start((SLA<<1)+I2C_READ);
		data=(i2c_read_ack()<<8);
		data|=i2c_read_nack();
		i2c_stop();
		if(calc(data)>22.0)PORTC^=(1<<PA7);
		PORTB^=(1<<PB0);
		_delay_ms(250);
	}
	return 0;
}
ISR ( USART_RXC_vect ){
char ReceivedByte ;
ReceivedByte = UDR ;
UDR = ReceivedByte ;
}
}
