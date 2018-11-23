#include "uart.h"


void uart_init(){

	UBRR1H = 0x00;//(uint8_t)(UBBR>>8);
	UBRR1L = 0x0C;//(uint8_t)UBBR;
	UCSR1B |= (1<<RXEN1)|(1<<TXEN1);
	UCSR1C |= (1<<UCSZ10)|(1<<UCSZ11);
}
void uart_transmitByte(uint8_t data ){
	UCSR1B ^= ((1<<RXEN1)|(1<<RXCIE1));
	while ( !( UCSR1A & (1<<UDRE1)) );
	UDR1 = data;
	UCSR1B ^= ((1<<RXEN1)|(1<<RXCIE1));
}
void uart_transmit(char *data){
	while(*data > 0){
		uart_transmitByte(*data++);
	}
}

void uart_prompt(void) {
	uart_transmit("\r\n>");
}
void uart_transmitNb(uint16_t nb){
	char str[]="00000";
	sprintf(str,"%u",nb);
	uart_transmit(str);

}

void uart_transmitNb(uint16_t nb, uint8_t mode) {
	if(mode=='d'){
		uart_transmitNb(nb);
	}else if(mode=='b'){
		char str[]="0b00000000";
		sprintf(str,"0b"BINTODECPATT,BINTODEC(nb));
		uart_transmit(str);
	}
}
