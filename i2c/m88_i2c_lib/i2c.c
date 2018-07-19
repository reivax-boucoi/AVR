#include "i2c.h"
#include <stdint.h>
#include <avr/io.h>
#include <util/twi.h>
#define F_I2C 400000UL

void ERROR(uint8_t err){
//panic !
}
void i2c_init(void){

  DDRC &= 0b11001111;
  PORTC &= 0b11001111;//disabling pull-ups !
  TWSR = 0;
#if F_CPU < 1600000UL // max of 400kHz if CPU frequency > 1.6MHz 
  TWBR = 0;
#else
  TWBR = ((F_CPU / F_I2C) - 16) / 2;
#endif
  TWCR = _BV(TWEN);
}

void i2c_start(void){
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
  while ((TWCR & _BV(TWINT)) == 0) ;
  switch(TW_STATUS){
    case TW_START:
    case TW_REP_START:
    break;
    default:
      ERROR(1);
	 break;
  }
}

void i2c_stop(void){
  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
  while (TWCR & _BV(TWSTO));
}


void i2c_putchar(uint8_t c){
  TWDR = c;
  TWCR = _BV(TWINT) | _BV(TWEN);
  while ((TWCR & _BV(TWINT)) == 0);
  switch(TW_STATUS){
	case TW_MT_SLA_ACK:
    case TW_MT_DATA_ACK:
    case TW_MR_SLA_ACK:
    case TW_MR_DATA_ACK:
      break;
    case TW_BUS_ERROR:
    case TW_MT_SLA_NACK:
    case TW_MT_DATA_NACK:
    case TW_MT_ARB_LOST:
    case TW_MR_SLA_NACK:
    case TW_MR_DATA_NACK:
	    ERROR(1);
	    break;
    default:
      ERROR(2);
	 break;
  }

}

uint8_t i2c_getchar_ack(void){
  TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN);
  while ((TWCR & _BV(TWINT)) == 0);
  return(TWDR);

}
uint8_t i2c_getchar_nack(void){
  TWCR = _BV(TWINT) | _BV(TWEN);
  while ((TWCR & _BV(TWINT)) == 0);
  return(TWDR);
}


void i2c_write(uint8_t addr, uint8_t len, uint8_t adrs, uint8_t *buf){ 

	i2c_start();
	i2c_putchar((addr<<1)|TW_WRITE);

	i2c_putchar(adrs);
	for(; len > 0; len--){
		i2c_putchar(*buf++);
	}
	i2c_stop();
}


void i2c_read(uint8_t addr, uint8_t len, uint8_t adrs, uint8_t *buf){

	i2c_start();
	i2c_putchar((addr<<1)|TW_WRITE);
	i2c_putchar(adrs);
	i2c_start();
	i2c_putchar((addr<<1)|TW_READ);
	for(; len > 1; len--){
		*(buf++) = i2c_getchar_ack();
	}
	*(buf++) = i2c_getchar_nack();
	i2c_stop();

}
