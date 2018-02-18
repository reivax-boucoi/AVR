#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "LCD.c"


unsigned int cnt;
char numString[4];
int pressed=0;

int main(void){

	LCDInit();
	
	DDRD &=~(1<<PORTD6);
	PORTD |= (1<<PIND6);
	
	while(1){
	
		if(bit_is_clear(PIND,6) && pressed==0){
			itoa(cnt++,numString,10);
			SendStringPos(0,0,numString);
			pressed=1;
		}else{
			pressed=0;
		}
	}
}

