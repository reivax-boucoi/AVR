#include <avr/io.h>
#include <util/delay.h>
#define SLA 0x40
#define TEMPREG 0xF3
#define RESETREG 0xFE
#define READ 0
#define WRITE 1

uint8_t i2c_start(void);
void i2c_stop(void);
uint8_t i2c_transmit(void);
void i2c_write(uint8_t addr,uint8_t command);
void i2c_writebyte(uint8_t addr,uint8_t command,uint8_t data);
uint8_t i2c_read(uint8_t command, uint8_t addr);
uint16_t readTemp(void);

float calc(uint16_t v);
int main(void){
	DDRB |= (1 << PB0) | (1<<PB1);
	PORTB |= (1<<PB0) | (0<PB1);
	TWBR=0x01;
    TWSR=(0<<TWPS1)|(0<<TWPS0);
	while(1){
		i2c_write(SLA,RESETREG);
		/*if(calc(readTemp())>55.0){
			PORTB |=(1<<PB0);
		}else{
			PORTB &=~(1<<PB0);
		}*/
		PORTB^=(1<<PB0);
		_delay_ms(500);

	}
	return 0;
}
uint8_t i2c_start(void) {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	PORTB &=~(1<<PB0);
	return (TWSR & 0xF8);
}

void i2c_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}
uint8_t i2c_transmit(void) {
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	return (TWSR & 0xF8);
}
void i2c_write(uint8_t addr,uint8_t command){
	  i2c_start();
	  TWDR = addr | WRITE;
	  i2c_transmit();
	  TWDR = command;
	  i2c_transmit();
	  i2c_stop();
	}
void i2c_writebyte(uint8_t addr,uint8_t command,uint8_t data) {
  i2c_start();
  TWDR = addr | WRITE;
  i2c_transmit();
  TWDR = command;
  i2c_transmit();
  TWDR = data;
  i2c_transmit();
  i2c_stop();
}
uint8_t i2c_read(uint8_t command, uint8_t addr){
	uint8_t data;
	i2c_start();
	TWDR = addr | WRITE;
	i2c_transmit();
	TWDR = command;
	i2c_transmit();
	i2c_start();
	TWDR = addr| READ;
	i2c_transmit();
	i2c_transmit();
	data=TWDR;
	i2c_stop();
	return data;
}
uint16_t readTemp(void){
	uint16_t data=0;
	i2c_start();
	TWDR = SLA | WRITE;
	i2c_transmit();
	TWDR = TEMPREG;
	i2c_transmit();
	i2c_start();
	TWDR = SLA| READ;
	i2c_transmit();
	i2c_transmit();
	data=TWDR<<8;
	i2c_transmit();
	data|=TWDR;
	i2c_stop();
	return data;
}
float calc(uint16_t v){
	return ((175.72*(float)v)/65536.0)-46.85;
}
