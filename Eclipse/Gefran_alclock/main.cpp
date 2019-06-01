#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Clock.h"
#include "Buzzer.h"

//Clock clock=Clock(2019,06,03,0,34,50);


int main(void){
	sei();
	clock.setAlarm(0,35,1);
	while(1){
		if(clock.sleepFlag){
			clock.gotoSleep();
		}
	}
	return 1;
}
ISR(TIMER2_OVF_vect){
	clock.addTimeSec();
}
ISR(TIMER0_COMPA_vect){
	clock.display.muxISR();
	clock.checkKeys();
}
ISR(PCINT2_vect){
	clock.sleepFlag=0;
	//if(!(KEYPIN & PWRCHECK))clock.sleepFlag=1;
}
ISR(TIMER1_COMPA_vect){
	clock.buzz.buzz_ISR();
}
