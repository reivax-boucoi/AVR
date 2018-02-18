#define F_CPU 1000000
#define TIMDIV 256

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD.h"
#include "LCD.c"


void printFloat(double val);

unsigned int cnt=0;
char numString[10];
unsigned int f=0;

int main(void){

	LCDInit();
	DDRC |= 0b00001000;
	
	PORTC &=~(1<<PINC3);
	
	
	TCCR0A = (1<<WGM01); // CTC on update
	OCR0A = 255; // update rate
	TIMSK0 =(1<<OCIE0A); // interrupt on compare match
	
	SendStringPos(11,1,"/1024");
	
	EICRA |= (1<<ISC01) | (1<< ISC00);
	EIMSK |= 1<<INT0;
	
	sei();		
	
	TCCR0B =(1<<CS02)|(0<<CS01)|(1<<CS00); // display update timer (/1024)
	
	//TCCR1B |= 1<<CS10 | 0<<CS11 | 1<<CS12; // f counter timer (/1024)
	//TCCR1B |= 0<<CS10 | 0<<CS11 | 1<<CS12; // f counter timer (/256)
	//TCCR1B |= 1<<CS10 | 1<<CS11 | 0<<CS12; // f counter timer (/64)
	//TCCR1B |= 0<<CS10 | 1<<CS11 | 0<<CS12; // f counter timer (/8)
	TCCR1B |= 1<<CS10 | 0<<CS11 | 0<<CS12; // f counter timer (/1)
	
	while(1){
	
	}

}

ISR(INT0_vect){
	
	f=TCNT1;
	TCNT1 = 0;
}

ISR(TIMER0_COMPA_vect){
	cnt++;
	if(cnt%3==0){
		cnt=0;
		itoa(/*976/*/f,numString,10); // 976,56 pour /1024 | 3906,25 pour /256 | 15625 pour /64 | 125000 pour /8 |1000000 pour /1
		SendStringPos(0,0,numString);
		SendString(" Hz  ");
	}
}
	