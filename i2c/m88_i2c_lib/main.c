#define F_CPU 16000000UL
#define F_I2C 400000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "i2c.h"

#define SCL PINC5
#define SDA PINC4
#define BAUD 9600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

#define ACC_addr 0x68


void uart_init (void);
void uart_transmit (uint8_t data);
uint8_t uart_recieve (void);
void uart_transmitMult(char *Data);

char myString[16];
uint8_t data[6] = {0};

int main(void){
	uart_init();
	uart_transmitMult("Initialized UART\n");
	i2c_init();
	uart_transmitMult("Initialized I2C\n");
	
	i2c_write(ACC_addr,1,0x6B,&data[0]);
	
	while(1){
		
		i2c_read(ACC_addr,6,0x3B,data);	
		_delay_ms(50);
		itoa((data[0] <<8) | data[1],myString,10);
		uart_transmitMult(myString);
		uart_transmit(',');
		itoa((data[2] <<8) | data[3],myString,10);
		uart_transmitMult(myString);
		uart_transmit(',');
		itoa((data[4] <<8) | data[5],myString,10);
		uart_transmitMult(myString);
		uart_transmit('\n');
	}
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