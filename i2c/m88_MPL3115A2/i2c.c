#include <stdint.h>
#include <avr/io.h>
#include <util/twi.h>

#define MAX_RESTARTS 20


void i2c_init(unsigned long i2c_speed){

  DDRC &= 0b11001111;
  PORTC &= 0b11001111;//disabling pull-ups !
  TWSR = 0;
#if F_CPU < 1600000UL // max of 400kHz if CPU frequency > 1.6MHz 
  TWBR = 0;
#else
  TWBR = ((F_CPU / i2c_speed) - 16) / 2;
#endif
  TWCR = _BV(TWEN);
}

uint8_t i2c_start(void){
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
  while ((TWCR & _BV(TWINT)) == 0) ;
  switch(TW_STATUS){
    case TW_START:
    case TW_REP_START:
      return(0);
    default:
      return(1);
  }
}

void i2c_stop(void){
  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
  while (TWCR & _BV(TWSTO));
}


uint8_t i2c_putchar(uint8_t c){
  TWDR = c;
  TWCR = _BV(TWINT) | _BV(TWEN);
  while ((TWCR & _BV(TWINT)) == 0);
  switch(TW_STATUS){
	case TW_MT_SLA_ACK:
    case TW_MT_DATA_ACK:
    case TW_MR_SLA_ACK:
    case TW_MR_DATA_ACK:
      return(0);
    case TW_BUS_ERROR:
    case TW_MT_SLA_NACK:
    case TW_MT_DATA_NACK:
    case TW_MT_ARB_LOST:
    case TW_MR_SLA_NACK:
    case TW_MR_DATA_NACK:
	    return(1);
    default:
      return(2);
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


uint8_t i2c_write(uint8_t addr, uint8_t len, uint8_t adrs, uint8_t *buf){ 
  uint8_t restarts = MAX_RESTARTS;
  if(len <= 0)
    return(0);
WriteRestart:
  if(i2c_start()){
    if(restarts-- == 0){
      i2c_stop();
      return(1);
    }
    goto WriteRestart;
  }
  if(i2c_putchar((addr<<1)|TW_WRITE)){
    if(restarts-- == 0){
      i2c_stop();
      return(2);
    }
    goto WriteRestart;
  }
  if(i2c_putchar(adrs)){
    return(3);
  }
  for(; len > 0; len--){
    if(i2c_putchar(*buf++)){
      return(4);
    }      
  }
  i2c_stop();
  return(0);
}


uint8_t i2c_read(uint8_t addr, uint8_t len, uint8_t adrs, uint8_t *buf){
  uint8_t restarts = MAX_RESTARTS;
  if(len == 0)
    return(0);
ReadRRestart:
  if(i2c_start()){
    if(--restarts == 0)
      return(1);
    goto ReadRRestart;
  }
  if(i2c_putchar((addr<<1)|TW_WRITE)){
    if(--restarts == 0){
      i2c_stop();
      return(2);
    }
    goto ReadRRestart;
  }
  if(i2c_putchar(adrs)){
    return(3);
  }  
  if(i2c_start()){
    return(4);
  }
  if(i2c_putchar((addr<<1)|TW_READ)){
    return(5);
  }
  for(; len > 1; len--){
    *(buf++) = i2c_getchar_ack();
  }
  *(buf++) = i2c_getchar_nack();
  i2c_stop();
  return(0);

}