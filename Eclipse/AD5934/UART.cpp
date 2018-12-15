/*
 * UART.cpp
 *
 *  Created on: Dec 15, 2018
 *      Author: user
 */

#include "UART.h"

UART::UART() {
	UBRR0H = (BAUDRATE>>8);
	UBRR0L = BAUDRATE;	// set baud rate
	UCSR0B|= (1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);	// enable receiver and transmitter
	UCSR0C|= (1<<UMSEL01)|(1<<UCSZ00)|(1<<UCSZ01);	// 8bit data format

}
void UART::transmitByte(char data){
    while (!( UCSR0A & (1<<UDRE0)));	// wait while register is free
    UDR0 = data;	// load data in the register
}
void UART::transmit(char *data){
	while(*data > 0){
		transmitByte(*data++);
	}
}
uint8_t UART::recieve(void){
    while(~(UCSR0A) & (1<<RXC0));	// wait while data is being received
    return UDR0;	// return 8-bit data
}
