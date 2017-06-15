#define F_CPU 20000000
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "LCD.c"

char textString[4];
unsigned long cnt=0;

int main(void){

	LCDInit();
	SendString("value = ");
	
	while(1){
	itoa(++cnt,textString,10);
	SendStringPos(8,0,textString);
	_delay_ms(1000);
	}
}

