#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Display.h"

volatile uint16_t i = 0;
volatile uint32_t counts = 0;
//T0=/4096
int main(void) {
	_delay_ms(200);
	initDisplay();
	setRange(1);
	displayNumber(-1);
	_delay_ms(250);
	DDRB &=~(1<<PINB2);
	TCCR1B |=1;//
	TIMSK1 |=(1<<TOIE1);
	sei();
	while (1)  {

	}
}


ISR(TIM1_OVF_vect){
	counts++;
	uint64_t total=(TCNT0+(counts<<8));
	/*uint8_t dp=3;
	if(total>999999999){
		setRange(2);
		if(total>99999999999){
			total/=1000000;
			dp=3;
		}else if(total>9999999999){
			total/=100000;
			dp=2;
		}else{
			total/=10000;
			dp=1;
		}
	}else if(total>999999){
		setRange(1);
		if(total>99999999){
			total/=1000;
			dp=3;
		}else if(total>9999999){
			total/=100;
			dp=2;
		}else{
			total/=10;
			dp=1;
		}
	}else{
		setRange(0);
	}*/
	displayNumberDP(total,dp);
	TCNT1=0x0000;
	counts=0;
}
