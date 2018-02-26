#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 76800
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
#define READ true
#define WRITE false

#define DATAPORT PORTC
#define DATAREG DDRC

#define ADDPORT1 PORTB
#define ADDPORT2 PORTA

#define WE PIND5
#define OE PIND6
void setMode(bool mode);
void setData(uint16_t a, uint8_t dat);
uint8_t readData(uint16_t a);
void uart_init (void);
void uart_transmit (unsigned char data);
unsigned char uart_recieve (void);
void uart_transmitMult(char *Data);
void printMemory(void);
int main(void){
	  uart_init();
	  sei();
	DATAREG = 0x00;
	DATAPORT = 0x00;
	DDRB = 0xFF;
	DDRA = 0xFF;
	DDRD |= (1<<PIND7)|(1<<WE)|(1<<OE);
	PORTD &=~(1<<PIND7);
	_delay_ms(1000);
	PORTD |=(1<<PIND7);
	_delay_ms(2000);

	setMode(READ);
	printMemory();
	PORTD &=~(1<<PIND7);
	_delay_ms(1000);
	PORTD |=(1<<PIND7);

	setMode(WRITE);
	for(uint16_t ad=0;ad<0x1FFF;ad++){
		//setData(ad,ad&0xFF);
		setData(ad,0x00);
	}
	PORTD &=~(1<<PIND7);
	while(1){
		PORTD^=(1<<PIND7);
		uart_transmitMult("beep\n");
		_delay_ms(500);

	}
	return 0;
}
ISR ( USART_RXC_vect ){
	char ReceivedByte ;
	ReceivedByte = UDR ;
	UDR = ReceivedByte ;
}
void printMemory(void){
	for (uint16_t base = 0; base <= 4095; base += 16) {
	    uint8_t data[16];
	    for (uint8_t offset = 0; offset <= 15; offset ++) {
	      data[offset] = readData(base + offset);
	    }

	    char buf[80];
	    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x\n",
	            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
	            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

	    uart_transmitMult(buf);
	}

}
inline void setMode(bool mode) {
	if(mode==READ){
		DATAREG = 0x00;
		DATAPORT = 0xFF;
		PORTD &=~(1<<OE);
		PORTD|=(1<<WE);
	}else if(mode==WRITE){
		PORTD |=(1<<OE)|(1<<WE);
		DATAREG = 0xFF;
	}
	_delay_ms(1);
}

inline void setData(uint16_t a, uint8_t dat) {
	ADDPORT1 = a & 0xFF;
	ADDPORT2 = a & 0x1F00;
	DATAPORT = dat;
	_delay_us(100);
	PORTD &=~(1<<WE);
	_delay_us(100);
	PORTD |=(1<<WE);
	_delay_us(100);
}

inline uint8_t readData(uint16_t a) {
	ADDPORT1 = a & 0xFF;
	ADDPORT2 = a & 0x1F00;
	_delay_us(1);
	return DATAPORT;
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
