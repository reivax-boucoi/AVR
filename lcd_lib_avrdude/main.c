#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "LCD.c"


uint8_t numX=0;
uint8_t numY=0;
char numString[3];

int main(void){

	LCDInit();
	SendStringPos(0,0,"x=");
	
	while(1){
	for(numY=0;numY<2;numY++){
		//SendString("x=");
		//SendStringPos(6,0,"y=");
			for(numX=0;numX<16;numX++){
			
				//itoa(numX,numString,10);
				//SendStringPos(3,0,numString);
				//itoa(numY,numString,10);
				//SendStringPos(9,0,numString);
				SetCursor(numX,numY);
				
				_delay_ms(500);
			}
		Send(0,0x01);
		_delay_ms(2);
		}
	}
}

