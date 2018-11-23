#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#define FOUT PA3 //T1 input
#define S0 PB3
#define S1 PA2
#define S3 PA1
#define S2 PA0

#define BINTODECPATT "%c%c%c%c%c%c%c%c"
#define BINTODEC(uint8_t)  \
  (uint8_t & 0x80 ? '1' : '0'), \
  (uint8_t & 0x40 ? '1' : '0'), \
  (uint8_t & 0x20 ? '1' : '0'), \
  (uint8_t & 0x10 ? '1' : '0'), \
  (uint8_t & 0x08 ? '1' : '0'), \
  (uint8_t & 0x04 ? '1' : '0'), \
  (uint8_t & 0x02 ? '1' : '0'), \
  (uint8_t & 0x01 ? '1' : '0')

void setSpeed(uint8_t speed){
	switch(speed){
	case 0://power down
		PORTB&=~(1<<S0);
		PORTA&=~(1<<S1);
		break;
	case 1://2%, 12kHz
		PORTB&=~(1<<S0);
		PORTA|=(1<<S1);
		break;
	case 2://20% 120kHz
		PORTB|=(1<<S0);
		PORTA&=~(1<<S1);
		break;
	case 3://100% 600kHz
		PORTB|=(1<<S0);
		PORTA|=(1<<S1);
		break;
	}
}
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
	_delay_us(1009);
	TCCR1B = 0;//stop counting

	if ((TIFR & (1<<TOV1)) !=0) {//OVF
		TIFR = (1<<TOV1);
		fin = 0xFFFF;
	}else{
		fin = 10*(TCNT1L+(TCNT1H<<8));
	}
	return fin;
}
uint8_t evaluate(uint16_t r,uint16_t g,uint16_t b){
	uint8_t bit=0;
	if(r>1300)bit|=(1<<2);
	if(g>1530)bit|=(1<<1);
	if(b>1765)bit|=(1<<0);
	return bit;
}
int main(void){
	DDRB|=(1<<PB0);
	DDRA&=~(1<<FOUT);
	DDRA|=(1<<S2)|(1<<S3)|(1<<S1);
	DDRB|=(1<<S0);
	setSpeed(3);
	uart_init();
	uart_transmit("Hi\r\n");
	uint16_t r,g,b;
	for(;;){
		setColor(0);
		_delay_ms(1);
		r=measure();
		setColor(1);
		_delay_ms(1);
		g=measure();
		setColor(2);
		_delay_ms(1);
		b=measure();

		char str[50]="";
//		sprintf(str,"%u,%u,%u,"BINTODECPATT"\r\n",r,g,b,BINTODEC(evaluate(r,g,b)));
		sprintf(str,"%u,%u,%u\r\n",r,g,b);
		uart_transmit(str);
		_delay_ms(100);

	}
}

