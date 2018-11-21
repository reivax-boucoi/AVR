#include "Led.h"
void setLed(uint8_t rgb, uint8_t val) {
	uart_transmit("\r\nSet ");
	switch(rgb){
	case 0:
		uart_transmit("red ");
		OCR1BL=val;
		break;
	case 1:
		uart_transmit("green ");
		OCR0B=val;
		break;
	case 2:
		uart_transmit("blue ");
		OCR1AL=val;
		break;
	default:
		uart_transmit("Oops");
		break;
	}
uart_transmit("channel led to ");
uart_transmitNb(val);
}

void setLed(uint8_t r, uint8_t g, uint8_t b) {
	OCR1BL=255-r;
	OCR0B=255-g;
	OCR1AL=255-b;

}

void initLed(void) {
	DDRA|=(1<<LEDR)|(1<<LEDG);
	DDRB|=(1<<LEDB);
	PORTA|=(1<<LEDR);
	PORTA|=(1<<LEDG);
	PORTB|=(1<<LEDB);
	TCCR0A|=(1<<COM0B1)|(1<<WGM00)|(1<<WGM01);
	TCCR1A|=(1<<COM1B1)|(1<<COM1A1)|(1<<WGM10);
	TCCR1B|=(1<<WGM12)|(1<<CS10);
	TCCR0B|=(1<<CS00);
	OCR0B =0xFF;//green
	OCR1AL=0xFF;//blue
	OCR1BL=0xFF;//red
	OCR1AH=0x00;
	OCR1BH=0x00;
}

void printLed(void) {
	uart_transmit("\r\nr= ");
	uart_transmitNb(255-OCR1BL);
	uart_transmit(", g= ");
	uart_transmitNb(255-OCR0B);
	uart_transmit(", b= ");
	uart_transmitNb(255-OCR1AL);
}
