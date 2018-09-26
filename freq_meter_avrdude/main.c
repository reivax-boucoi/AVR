#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "LCD.c"


unsigned int cnt=1953;
char numString[4];


int main(void){

	LCDInit();
	DDRD |= 0b00000100;
	
	PORTD &=~(1<<PIND2);
	
	TCCR1B |= 0<<CS10 | 0<<CS11 | 1<<CS12;
	
	
	while(1){
		if (TCNT1 > cnt){//1953
			cnt--;
			itoa(TCNT1,numString,10);
			SendStringPos(0,0,numString);
			TCNT1 = 0;
			
			PORTD ^=(1<<PIND2);
		}
	}
}




