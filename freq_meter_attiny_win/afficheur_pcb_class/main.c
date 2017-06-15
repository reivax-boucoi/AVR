#define F_CPU 1000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Display.h"

volatile i=0;
Display D;
int main(void){
	
	TIMSK|=1<<TOIE1;
	
	sei();
	
	TCCR1|=1<<CS13|1<<CS12|0<<CS10; // counter timer
	TCCR0B|=1<<CS01;
	
	while(1){
	}
	return 0;
}

ISR(TIMER0_OVF_vect){
	//D.displaySingle(D.n,D.nums[D.n]);
	//D.n=(D.n+1)%6;
}

ISR(TIMER1_OVF_vect){
//D.displayNumber(i++);
	
}