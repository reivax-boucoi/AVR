#define F_CPU 18432000UL
#define F_I2C 400000UL

#include <avr/io.h>
#include <util/delay.h>

#define SCL PINC5
#define SDA PINC4
#define MAX_RESTARTS 20
#define BAUD 9600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

#define ACC_addr 0x68

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

char myString[16];
uint8_t data[14] = {0};
int64_t awg[7] = {0};


int main(void){
	uart_init();
	uart_transmitMult("Initialized UART\n");
	i2c_init();
	uart_transmitMult("Initialized I2C\n");

	i2c_write(ACC_addr,1,0x6B,&data[0]);

	while(1){
	uint8_t j;
	for( j=0;j<50;j++){
		i2c_read(ACC_addr,14,0x3B,data);
		uint8_t i;
		for( i=0;i<7;i++){
		awg[i]+=data[2*i]<<8 | data[2*i +1];
	}
	_delay_ms(2);
	}
	for( j=0;j<7;j++){
		awg[j]/=50;
		itoa(awg[j],myString,10);
		uart_transmitMult(myString);
		if(j<6)uart_transmit(',');
	}
	uart_transmit('\r');
	}
}

void i2c_init(){

  DDRC &= ~((1<<SCL) |(1<<SDA));
  PORTC &= ~((1<<SCL) |(1<<SDA));//disabling pull-ups !
  TWSR = 0;
#if F_CPU < 1600000UL // max of 400kHz if CPU frequency > 1.6MHz
  TWBR = 0;
#else
  TWBR = ((F_CPU / F_I2C) - 16) / 2;
#endif
  TWCR = 1<<TWEN;
}

uint8_t i2c_start(void){
  TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
  while ((TWCR & (1<<TWINT)) == 0) ;
  switch(TWSR & ((1<<TWS7)|(1<<TWS6)|(1<<TWS5)|(1<<TWS4)|(1<<TWS3))){
    case 0x08:
    case 0x10:
      return(0);
    default:
      return(1);
  }
}

void i2c_stop(void){
  TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
  while (TWCR & (1<<TWSTO));
}

uint8_t i2c_putchar(uint8_t c){
  TWDR = c;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while ((TWCR & (1<<TWINT)) == 0);
  switch(TWSR & ((1<<TWS7)|(1<<TWS6)|(1<<TWS5)|(1<<TWS4)|(1<<TWS3))){
	case 0x18:
    case 0x28:
    case 0x40:
    case 0x50:
      return(0);
    case 0x00:
    case 0x20:
    case 0x30:
    case 0x38:
    case 0x48:
    case 0x58:
	    return(1);
    default:
      return(2);
  }

}

uint8_t i2c_getchar_ack(void){
  TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
  while ((TWCR &(1<<TWINT)) == 0);
  return(TWDR);

}

uint8_t i2c_getchar_nack(void){
  TWCR = (1<<TWINT) | (1<<TWEN);
  while ((TWCR & (1<<TWINT) ) == 0);
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
  if(i2c_putchar((addr<<1)|0x00)){
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
  if(i2c_putchar((addr<<1)|0x00)){
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
  if(i2c_putchar((addr<<1)|0x01)){
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
