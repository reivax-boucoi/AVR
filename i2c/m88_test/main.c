#define F_CPU 12000000UL
#define F_I2C 400000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define SCL PINC5
#define SDA PINC4
#define ADC_addr 0x68 //-- a confirmer

#define WRITE(a) (#a<<1)
#define READ(a) (#a<<1)|1

#define START 0x08 
#define MT_SLA_ACK 0x18
#define MT_DATA_ACK 0x28
#define MR_SLA_ACK 0x40
#define MR_DATA_ACK 0x50
//  pull-up resistors 4.7k !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void i2c_init (void);
void ERROR();
void i2c_transmit(uint8_t reg, uint8_t data);
uint8_t i2c_receive();

int main(void){

  DDRB |= 1<<PINB0;
  PORTB &= ~(1<<PINB0);
  i2c_init();
  
  while(1){
	/*PORTB |= 1<<PINB0;
	_delay_ms(20);
	PORTB &= ~(1<<PINB0);
	_delay_ms(980);*/
  }
  return 0;
}
void ERROR(){
	PORTB |=1<<PINB0;
	while(1);
}
void i2c_init (void){
	DDRC &=~( 1<<SCL | 1<<SDA );
	PORTC &=~( 1<<SCL | 1<<SDA ); // disable pull-ups if slave device is 3.3V
	//PRRn.PRTWI to 0 in TWI0
	TWSR = 0; // reset
	
	#if F_CPU<1600000
		TWBR = 0; // no prescaler, clock <16MHz
	#else
		TWBR = ((F_CPU-F_I2C) -16) /2;
	#endif
	
    TWCR = (1<<TWEN); // enable
	
}

void i2c_transmit(uint8_t reg, uint8_t data){

	//start -> TWCR ( ans TWINT has to be set)
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	
	//check TWSR to see if start was sent 
	while (!(TWCR &(1<<TWINT)));
	if ((TWSR & 0xF8) !=START) ERROR();
	
	//adress : XXXXXXX + W + ACK from slave -> TWDR load SLA+W + control TWCR : TWINT=1 and TWSTA=0
	TWDR =(ADC_addr<<1);// ADC_addr<<1 | 0(write)

	TWCR = (1<<TWINT) | (1<<TWEN);
	// check TWSR for sla+w and ack received
	while (!(TWCR &(1<<TWINT)));
	if ((TWSR & 0xF8) != MT_SLA_ACK) ERROR();
	
	//data: XXXXXXXX + ACK from slave -> TWDR load data + control TWCR : TWINT=1
	TWDR = data;
	TWCR = (1<<TWINT) |(1<<TWEN);
	
	//check TWSR for data and ack
	while (!(TWCR &(1<<TWINT)));
	if ((TWSR & 0xF8) !=MT_DATA_ACK) ERROR();
	
	//stop-> TWCR control : TWINT=1
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

uint8_t i2c_receive(){
	uint8_t data;
	//start
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	
	while (!(TWCR &(1<<TWINT)));
	if ((TWSR & 0xF8) !=START) ERROR();
	
	//adress : XXXXXXX + R +ACK from slave
	TWDR = (ADC_addr<<1)|1;
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	while (!(TWCR &(1<<TWINT)));
	if ((TWSR & 0xF8) != MT_SLA_ACK) ERROR();
	
	//repeated start
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	
	//read 8bits, then ACK from master -> si nack ?
	while (!(TWCR &(1<<TWINT)));
	if ((TWSR & 0xF8) != MR_SLA_ACK) ERROR();
	
	data=TWDR;
	TWCR = (1<<TWINT)|(1<<TWEN);
	
	while (!(TWCR &(1<<TWINT)));
	if ((TWSR & 0xF8) != MR_DATA_ACK) ERROR();
	
	//send ack
	TWCR|= (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while (!(TWCR &(1<<TWINT)));
	
	//stop
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	
	return data;
}
