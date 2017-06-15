#define F_CPU 4000000UL
#define F_I2C 400000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <stdlib.h>
/*
#define SCL PINC5
#define SDA PINC4*/
#define MAX_RESTARTS 20
#define BAUD 9600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

#define BAR_addr 0x60

void i2c_init(void);
uint8_t i2c_start(void);
void i2c_stop(void);
uint8_t i2c_putchar(uint8_t c);
uint8_t i2c_getchar_ack(void);
uint8_t i2c_getchar_nack(void);
uint8_t i2c_write(uint8_t addr, uint8_t len, uint8_t adrs, uint8_t *buf);
uint8_t i2c_read(uint8_t addr, uint8_t len, uint8_t adrs, uint8_t *buf);
void uart_init (void);
void uart_transmit (uint8_t data);
uint8_t uart_recieve (void);
void uart_transmitMult(char *Data);

char myString[25];
uint8_t data[5] = {0};

int main(void){
	uart_init();
	uart_transmitMult("Initialized UART\r");
	i2c_init();
	uart_transmitMult("Initialized I2C\r");
	data[0]=0xB8;
	i2c_write(BAR_addr,1,0x28,&data[0]);
	data[0]=0x07;
	i2c_write(BAR_addr,1,0x13,&data[0]);
	data[0]=0xB9;
	i2c_write(BAR_addr,1,0x28,&data[0]);
	uart_transmitMult("Set up sensor\r");

	while(1){

		i2c_read(BAR_addr,1,0x00,&data[0]);
		if(data[0] && 0x08){
			i2c_read(BAR_addr,5,0x01,data);
			uint32_t pressure = (data[0] <<8) | data[1];
			pressure = (pressure <<8) | data[2];
			itoa(pressure,myString,10);
			uart_transmitMult(myString);
			uart_transmit(',');
			itoa((data[3] <<8) | data[4],myString,10);
			uart_transmitMult(myString);
			uart_transmit('\r');
		}else{
	uart_transmitMult("Sensor not available\r");
		//not currently available
		}
		_delay_ms(100);
	}
}


void i2c_init(){

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
	if(len <= 0)return(0);
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

void uart_init (void){
    UBRR0H = (BAUDRATE>>8);
    UBRR0L = BAUDRATE;	// set baud rate
    UCSR0B|= (1<<TXEN0)|(1<<RXEN0);	// enable receiver and transmitter
    UCSR0C|= (1<<UMSEL01)|(1<<UCSZ00)|(1<<UCSZ01);	// 8bit data format
}

void uart_transmit (uint8_t data){
    while (!( UCSR0A & (1<<UDRE0)));	// wait while register is free
    UDR0 = data;	// load data in the register
}

uint8_t uart_recieve (void){
    while(!(UCSR0A) & (1<<RXC0));	// wait while data is being received
    return UDR0;	// return 8-bit data
}

void uart_transmitMult(char* data){
	while(*data>0){
		uart_transmit(*data++);
	}
}
