#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 38400
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
#define READ true
#define WRITE false

#define DATAPORT PORTC
#define DATAREAD PINC
#define DATAREG DDRC
//A0=>PINB7, .. ,A7=>PINB0,A8=>PINA0,..,A14=>PINA6
#define ADDPORT1 PORTB
#define ADDPORT2 PORTA

#define WE PD4
#define OE PD3
#define CE PD2
//#define WE PD5
//#define OE PD6

void setMode(bool mode);
void setData(uint16_t a, uint8_t dat);
uint8_t readData(uint16_t a);
void printMemory(uint8_t l);
uint8_t reverse(uint8_t b);
void uart_init (void);
void uart_transmit (unsigned char data);
unsigned char uart_recieve (void);
void uart_transmitMult(char *Data);
int main(void){
	  uart_init();
	  sei();
	DATAREG = 0x00;
	DATAPORT = 0x00;
	DDRB = 0xFF;
	DDRA = 0xFF;
	DDRD |= (1<<PD7)|(1<<WE)|(1<<OE);
	PORTD &=~(1<<PD7);
	_delay_ms(2000);

	setMode(READ);
	printMemory(8);
	setMode(WRITE);
	for(uint8_t i=0;i<255;i++){
		setData(i,i);
	}
	setMode(READ);
	printMemory(8);
	PORTD &=~(1<<PD7);
	DATAREG=0x00;
	while(1){
		PORTD^=(1<<PD7);
		uart_transmitMult("beep\r\n");
		_delay_ms(2000);

	}
	return 0;
}
ISR ( USART_RXC_vect ){
	char ReceivedByte ;
	ReceivedByte = UDR ;
	UDR = ReceivedByte ;
}
uint8_t reverse(uint8_t b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}
void printMemory(uint8_t l){
	for (uint16_t base = 0; base < (uint16_t)(1<<l); base += 16) {
	    uint8_t data[16];
	    for (uint8_t off = 0; off <= 15; off ++) {
	      data[off] = readData(base + off);
	    }

	    char buf[80];
	    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x\r\n",
	            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
	            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

	    uart_transmitMult(buf);
	}

}
void setMode(bool mode) {
	PORTD &=~(1<<CE);
	if(mode==READ){
		DATAREG = 0x00;
		PORTD &=~(1<<OE);
	}else if(mode==WRITE){
		PORTD |=(1<<OE);
		DATAREG = 0xFF;
	}
	DATAPORT = 0x00;
	PORTD |=(1<<WE);
	_delay_ms(10);
}


//A0=>PINB7, .. ,A7=>PINB0,A8=>PINA0,..,A14=>PINA6
void setData(uint16_t a, uint8_t dat) {
	ADDPORT1 = reverse(a & 0xFF);
	ADDPORT2 = (a & 0x7F00)>>8;
	DATAPORT = dat;
	PORTD &=~(1<<WE);
	_delay_us(1);
	PORTD |=(1<<WE);
}

uint8_t readData(uint16_t a) {
	ADDPORT1 = reverse(a) & 0xFF;
	ADDPORT2 = a & 0x7F00;
	_delay_us(1);
	return DATAREAD;
}
void uart_init (void){
   UBRRH = (BAUDRATE>>8);
   UBRRL = BAUDRATE;
   UCSRB|= (1<<TXEN)|(1<<RXEN);
   UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
   UCSRB |= (1 << RXCIE );
}
void uart_transmit (unsigned char data){
   while (!( UCSRA & (1<<UDRE)));
   UDR = data;
}
unsigned char uart_recieve (void){
   while(!(UCSRA) & (1<<RXC));
   return UDR;
}
void uart_transmitMult(char *Data){
	while(*Data>0){
		uart_transmit(*Data++);
	}
}
