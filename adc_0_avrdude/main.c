#define F_CPU 1000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LCD.h"
#include "LCD.c"

int x;
char numString[4];

int main(void){

	LCDInit();
	
	ADMUX |= (1<<REFS0) | (1<<MUX2) | (1<<MUX0);// Vref=Vcc, from ADC5
	ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // enable, enable interrupts, prescaler to 128
	DIDR0 |= (1<<ADC5D); // make sure no gigital output is set
	sei();
	ADCSRA |= (1<<ADSC);
	ADCSRA |= (1<<ADSC);
	
	while(1){
	
	}
}
ISR(ADC_vect){ //isr adc complete

	x=ADC;//getting directly the 10bits value
	itoa(x,numString,10);
	SendStringPos(0,0,"    ");
	SendStringPos(0,0,numString);
	ADCSRA |= (1<<ADSC); // start conversion again
}
