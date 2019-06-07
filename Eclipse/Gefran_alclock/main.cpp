#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Clock.h"
#include "Buzzer.h"


int main(void){
	sei();
	clock.setAlarm(9,10,1,0);
	clock.setAlarm(9,12,1,1);

	_delay_ms(250);
	if(!(KEYPIN & PWRCHECK))clock.sleepFlag=1;

	while(1){
		if(clock.sleepFlag){
			clock.gotoSleep();
		}
	}
	return 1;
}
ISR(TIMER2_OVF_vect){
	clock.addTimeSec();
	if(clock.alarmisRunning()){
		clock.buzz.toggle();
	}
}
ISR(TIMER0_COMPA_vect){
	clock.display.muxISR();
	clock.checkKeys();
}
ISR(PCINT2_vect){
	clock.sleepFlag=0;
	if((!(KEYPIN & PWRCHECK))&& (KEYPIN & PWRKEY))clock.sleepFlag=1;
}
ISR(TIMER1_COMPA_vect){
	clock.buzz.buzz_ISR();
}
