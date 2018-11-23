#include "Led.h"
static char mode='u';//u:user set, r:randomize, c:cycle
static
void setLed(uint8_t rgb, uint8_t val) {
	uart_transmit("\r\nSet ");
	switch(rgb){
	case 0:
		uart_transmit("red ");
		OCRR=val;
		break;
	case 1:
		uart_transmit("green ");
		OCRG=val;
		break;
	case 2:
		uart_transmit("blue ");
		OCRB=val;
		break;
	default:
		uart_transmit("Oops");
		break;
	}
uart_transmit("channel led to ");
uart_transmitNb(255-val);
}

void setLed(uint8_t r, uint8_t g, uint8_t b) {
	OCRR=255-r;
	OCRG=255-g;
	OCRB=255-b;

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
	OCRR =0xFF;//red
	OCRB=0xFF;//blue
	OCRG=0xFF;//green
	OCR1AH=0x00;
	OCR1BH=0x00;
}

void printLed(void) {
	uart_transmit("\r\nr= ");
	uart_transmitNb(255-OCRR);
	uart_transmit(", g= ");
	uart_transmitNb(255-OCRG);
	uart_transmit(", b= ");
	uart_transmitNb(255-OCRB);
	switch(mode){
	case 'u':
		uart_transmit(", user set");
		break;
	case 'r':
		uart_transmit(", randomized");
		break;
	case 'c':
		uart_transmit(", cycle");
		break;
	}
}

void randomize(void) {

}

void cycle(void) {
}
