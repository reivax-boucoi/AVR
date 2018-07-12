#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD.h"
#include "LCD.c"


unsigned int cnt=0;
unsigned char mod=0;
char numString[10];

int main(void){

	LCDInit();
	
	
	TCCR0A = (1<<WGM01); // CTC on update
	OCR0A = 124; // update rate
	TIMSK0 =(1<<OCIE0A); // interrupt on compare match
	
	EICRA |= (1<<ISC01) | (1<< ISC00);
	EIMSK |= 1<<INT0;
	
	sei();		
	
	TCCR0B =(0<<CS02)|(1<<CS01)|(1<<CS00); // display update timer (/64)
	
	while(1){
	
	}

}

ISR(INT0_vect){
	if(cnt==0){
		TCCR0B =(0<<CS02)|(1<<CS01)|(1<<CS00); //demarre timer (/64)
	}
	cnt++;
}

ISR(TIMER0_COMPA_vect){
		
	mod++; // timer increment
	
	if(mod%124==0){ // timer stop if 1sec
		mod=0; 
		itoa(cnt,numString,10);
		SendStringPos(0,0,numString);
		SendString("   ");
		cnt=0; // reset count
		TCCR0B=0x00; // stop timer until new rising edge
	}
}
	