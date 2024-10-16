#define F_CPU 12000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
//#include <avr/interrupt.h>

#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR
#define string "hello world\n"
void uart_init (void);
void uart_transmit (uint8_t data);
uint8_t uart_recieve (void);
void uart_transmitMult(char *Data);


int main(void){

  DDRB |= 1<<PINB0;
  _delay_ms(5000);
  uart_init();
  uint16_t cnt=0;
  uint8_t myByte,my1Byte;
  myByte = eeprom_read_byte((uint8_t*)46);
  //my1Byte = eeprom_read_byte((uint8_t*)47);
if(myByte<33 ||myByte>127){
myByte=35;
}
  
  while(1){
	PORTB |= 1<<PINB0;
	_delay_ms(20);
	PORTB &= ~(1<<PINB0);
	_delay_ms(2000);
	myByte=cnt+48;
	cnt++;
	uart_transmit(myByte);
	//uart_transmit(my1Byte);
	uart_transmit(0x21);
  eeprom_update_byte((uint8_t*)46,myByte);
  }
  return 0;
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