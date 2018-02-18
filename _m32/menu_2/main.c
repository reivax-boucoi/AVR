#define nbButton 3
#define F_CPU 20000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Button.h"
#include "LCD.h"
#include "LCD.c"

char pos=0;
int cnt=0;
char select=0;
char menu [4][15] = {"1er menu      ","2nd menu      ","3eme menu     ","4eme menu     "};
char subMenu [4][15] = {"Frequency =   ","count =       ","Select WF:    ","Value =       "};
char nbString[6];
void LCDMenuUpdate(void);
void updateFunc(void);

int main(void){
	
	LCDInit();
	
	DDRB = 0b00000000;
	PORTB = 0b00000111;
	
	TCCR0 |= (1<<FOC0) | (1<<CS02) | (1<<CS00);
	
	setConfidenceLevel(0,100);
	setConfidenceLevel(1,100);
	setConfidenceLevel(2,100);
	LCDMenuUpdate();
	
	
	
	while(1){
		
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
		itoa(TCNT0,nbString,10);
		SendStringPos(11,0,nbString);		
	break;

	case (1):
	break;

	case (2):
		
	break;
	case (3):
		
	break;
	}

	SendStringPos(0,1,"               ");
	}
}

void updateFunc(void){
	
	
}