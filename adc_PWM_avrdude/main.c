#define F_CPU 1000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LCD.h"
#include "LCD.c"

int x;
//double DCycle=50;
char numString[4];

int main(void){

	LCDInit();
	
	DDRD = (1 << PORTD6);
	//TCCR0A|=(1<<COM0A1)|(1<<WGM00)|(1<<WGM01)|(1<<WGM02);
	//TIMSK0|=(1<<TOIE0);
	//OCR0A=(DCycle/100.0)*255.0;
	
	ADMUX |= (1<<REFS0) | (1<<MUX2) | (1<<MUX0);// Vref=Vcc, from ADC5
	ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // enable, enable interrupts, prescaler to 128
	DIDR0 |= (1<<ADC5D); // make sure no digital output is set
	sei();
	
	//TCCR0B|=(1<<CS02)|(1<<CS00);
	ADCSRA |= (1<<ADSC);
	
	while(1){
	
		_delay_us(256-x);
		PORTD |=(1<<PIND6);
		_delay_us(x);
		PORTD &=~(1<<PIND6);
	}
}

ISR(ADC_vect){ //isr adc complete
	
	x=ADC; //getting directly the 10bits value
	//DCycle=x/4;
	itoa(x,numString,10);
	SendStringPos(0,0,"ADC=    ");
	SendStringPos(4,0,numString);
	
	//SendStringPos(9,0,"DC=      ");
	//itoa(DCycle,numString,10);
	//SendStringPos(12,0,numString);
	ADCSRA |= (1<<ADSC); // start conversion again
}
/*ISR(TIMER0_OVF_vect){
	
	OCR0A=(DCycle/100.0)*255.0;

}*/