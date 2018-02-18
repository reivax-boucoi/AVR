#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "LCD.c"


float pi = 3.1415926535;

int main(void){

	LCDInit();
	SendStringPos(0,0,"3.1415926535");
	SendFloatPos(0,1,pi);
	
	while(1){
	}
}

