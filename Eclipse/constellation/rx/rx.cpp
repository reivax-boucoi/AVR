#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#define FIN PA3
#define S2 PA5 //TODO check S2 & S3 for reverse connection
#define S3 PA6


void setColor(uint8_t rgb){//0:r 1:g 2:b 3:w
	switch(rgb){
	case 0:
		PORTA&=~(1<<S2);
		PORTA&=~(1<<S3);
		break;
	case 1:
		PORTA|=(1<<S2);
		PORTA|=(1<<S3);
		break;
	case 2:
		PORTA&=~(1<<S2);
		PORTA|=(1<<S3);
		break;
	case 3:
		PORTA&=~(1<<S3);
		PORTA|=(1<<S2);
		break;
	}
}
uint16_t measure(void){
	uint16_t fin;
	TCNT1H = 0x00;//reset
	TCNT1L = 0x00;
	TCCR1B = (1<<CS12)|(1<<CS11);	// T1 input start

	while (TCNT1L == 0);
	_delay_us(100900);
	TCCR1B = 0;//stop counting

	if ((TIFR & (1<<TOV1)) !=0) {//OVF
		TIFR = (1<<TOV1);
		fin = 0xFFFF;
	}else{
		fin = 10*(TCNT1L+(TCNT1H<<8));
	}
	return fin;
}
int main(void){
	DDRB|=(1<<PB0);
	DDRA&=~(1<<FIN);
	DDRA|=(1<<S2)|(1<<S3);
	uart_init();
	uart_transmit("Hi\r\n");
	uint16_t r,g,b;
	for(;;){
		setColor(0);
		_delay_ms(2);
		r=measure();
		setColor(1);
		_delay_ms(2);
		g=measure();
		setColor(2);
		_delay_ms(2);
		b=measure();
		uart_transmitNb(r);
		uart_transmitByte(',');
		uart_transmitNb(g);
		uart_transmitByte(',');
		uart_transmitNb(b);
		uart_transmit("\r\n");
	}
}

