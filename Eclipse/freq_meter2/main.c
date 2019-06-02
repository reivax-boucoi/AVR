//reciprocal method attempt
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Display.h"

static volatile uint32_t tim0_ovf_cnt = 0;
static volatile uint32_t int0_cnt = 0;

static uint32_t nb_events=1000000UL;
static uint32_t i=0;

//T0=/4096

static uint32_t getElapsedTicks(void){
    uint8_t t;
    uint32_t m;
    m = tim0_ovf_cnt;
    t = TCNT0;
    if ((TIFR0&(1<<TOV0)) && t < 255) {
    	m++;
    }
    return (m << 8) | t;
}

int main(void) {
	_delay_ms(200);
	initDisplay();
	setRange(1);
	displayNumber(-1);
	_delay_ms(250);

	DDRB &=~(1<<PINB2);
	TCCR0B|=(1<<CS01)|(1<<CS00);//64
	TIMSK0 |=(1<<TOIE0);

	MCUCR|=(1<<ISC01)|(1<<ISC00);
	GIMSK|=(1<<INT0);


	while (1)  {
		_delay_ms(500);
		if(int0_cnt>nb_events){

		}else{

		}
		displayNumberDP(i++,i%6);
	}
}
ISR(INT0_vect){
	int0_cnt++;
}

ISR(TIM0_OVF_vect){
	tim0_ovf_cnt++;
}
