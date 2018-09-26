#define nbButton 3
#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Button.h"
#include "LCD.h"
#include "LCD.c"

char pos=0;
char cnt=0;
int value=0;
char select=0;
char menu [4][15] = {"1er menu  ","2nd menu  ","3eme menu  ","4eme menu  "};
char subMenu [4][15] = {"Frequency =   ","count =       ","Select WF:    ","Value =       "};
char nbString[4];
void LCDMenuUpdate(void);
void updateFunc(void);

int main(void){

	DDRB = 0b00000000;
	PORTB = 0b00000111;
	
	ADMUX |= (1<<REFS0) ;// Vref=Vcc, from ADC0
	ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2); // enable, enable interrupts, prescaler to 128
	sei();
	
	ADCSRA |= (1<<ADSC);
	
	LCDInit();
	setConfidenceLevel(0,100);
	setConfidenceLevel(1,100);
	setConfidenceLevel(2,100);
	LCDMenuUpdate();
	
	
	
	while(1){
		
		updateFunc();
		
		if(ButtonPressed(0,PINB,0)){
			if(!select){
				pos++;
			}else{
				cnt++;
			}
		LCDMenuUpdate();
		
		}else if(ButtonPressed(1,PINB,1)){
			if(!select){
				pos--;
			}else{
				cnt--;
			}
		LCDMenuUpdate();
		
		}else if(ButtonPressed(2,PINB,2)){
			select=1-select;
		LCDMenuUpdate();
		}
	}
}

void LCDMenuUpdate(void){
	if(!select){ // if in general menu
	
		SendStringPos(0,0,">");
		if(pos<0){pos=3;}
		pos=pos%4;
		
		if(pos<3){
			SendStringPos(1,1,menu[pos+1]);
		}else{
			SendStringPos(1,1,menu[0]);
		}
		SendStringPos(1,0,menu[pos]); // show selected menu
		
	}else{//	if in submenu
	
	SendStringPos(0,0,subMenu[pos]);

	switch (pos){
	case (0):
		
	break;

	case (1):
	
	break;

	case (2):
		
	break;
	case (3):
		itoa(value,nbString,10);
		SendStringPos(9,0,nbString);
	break;

	default:
		
	break;
	}

	SendStringPos(0,1,"               ");
	}
}

void updateFunc(void){
	
	ADCSRA |= (1<<ADSC);
	LCDMenuUpdate();
	
}

ISR(ADC_vect){ //isr adc complete

	value=ADC;
}