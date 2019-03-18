#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Display.h"

#define CALF 953.975
static const float calFt[4]={256.0*CALF,32.0*CALF,4.0*CALF,CALF};
volatile uint16_t i = 0;
volatile uint32_t counts = 0;
uint8_t gate_ps=4;

void setGateTime(uint8_t incdec){
	if(gate_ps<4 && incdec)gate_ps++;
	if(gate_ps>1 && !incdec)gate_ps--;
	TCCR1B |=gate_ps;
}

int main(void) {
	_delay_ms(200);
	initDisplay();
	setRange(1);
	displayNumber(-1);
	_delay_ms(250);
	DDRB &=~(1<<PINB2);//T0=/4096
	TCCR1B |=gate_ps;//(1<<CS12);//|(1<<CS10);//gate timer
	TIMSK0|=(1<<TOIE0);
	TCCR0B |=(1<<CS02)|(1<<CS01)|(1<<CS00); //T0 counter
	TIMSK1 |=(1<<TOIE1);
	sei();
	while (1)  {

	}
}
ISR(TIM0_OVF_vect){ // input overflow
	counts++;
}

ISR(TIM1_OVF_vect){

	//uint16_t div = 0.524288;//F_CPU/(1024*256*1)UL; // F_CPU / (prescaler*T/C res°*UpdateSlowDown)
	uint64_t total=(TCNT0+(counts<<8))<<12;
	uint8_t dp=3;
	total*=calFt[gate_ps-1];//total in mHz
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
	}
	displayNumberDP(total,dp);
	TCNT1=0x0000;
	TCNT0=0x00;
	counts=0;
}
