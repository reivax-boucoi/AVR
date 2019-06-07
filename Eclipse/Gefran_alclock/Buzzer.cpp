#include "Buzzer.h"

Buzzer::Buzzer() {
	BREG|=PP|PN;
	BPORT&=~(PP|PN);
	TCNT1=0;
	OCR1A=0x03E0;
	TCCR1B|=(1<<WGM12);
	TIMSK1|=(1<<OCIE1A);
	started=false;
}
void Buzzer::buzz_ISR(void){
	if(BPORT & PP){
		BPORT|= PN;
		BPORT&=~PP;
	}else{
		BPORT|= PP;
		BPORT&=~PN;
	}
}

void Buzzer::toggle(void){
	if((TCCR1B & BPSCL)){
		start();
	}else{
		stop();
	}
}
void Buzzer::stop(void){
	BPORT&=~(PP|PN);
	TCCR1B &=~BPSCL;
	started=false;
}

void Buzzer::start(void){
	TCCR1B |=BPSCL;
	started=true;
}
Buzzer::~Buzzer() {
}

