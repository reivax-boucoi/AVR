#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Clock.h"


Clock clock=Clock(2019,06,03,0,34,56);
uint8_t i=0;

int main(void){

	sei();
	clock.setAlarm(6,55,1);
	while(1){
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
	//handle wakeup here
}
ISR(TIMER1_COMPA_vect){
	clock.buzz.buzz_ISR();
}
