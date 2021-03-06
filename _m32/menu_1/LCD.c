#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "LCD.h"


void LCDInit(){

	LCDControlDIR |= 1<<Enable | 1<<RW | 1<<RS;
	_delay_ms(15);
	Send(0,0x01); //Clear display + set cursor at home pos�
	_delay_ms(2);
	Send(0,0x38); // set interface to 8bits, nb disp lines to 2, disp mode 5*8
	_delay_us(50);
	Send(0,0x0E); // set display on, cursor on, blinking off
	_delay_us(50);
	
}


void toggle(){
	LCDControlPORT |= 1<<Enable;
	asm volatile ("nop");
	asm volatile ("nop");
	LCDControlPORT &= ~1<<Enable;
}

void Send(unsigned char state, unsigned char data){
	LCDDataDIR = 0; // set data reg as input
	LCDControlPORT |= 1<<RW;
	LCDControlPORT &= ~1<<RS;

	while (LCDDataPORT >= 0x80){ // check if LCD available
		toggle();
	}
	LCDDataDIR = 0xFF; //back to output
	
	LCDDataPORT = data; // put data
	if(!state){ // set control as command
	LCDControlPORT &= ~ ((1<<RW)|(1<<RS));
	
	}else{ // set control as text
	
	LCDControlPORT &= ~ (1<<RW);
	LCDControlPORT |= 1<<RS;
	}
	toggle(); // write
	LCDDataPORT = 0; // clear reg
}



void SendString(char *String){ // send each char one by one 
	while(*String>0){
		Send(1,*String++);
	}

}
void SendStringPos(int x, int y, char *String){
	SetCursor(x,y);
	while(*String>0){
		Send(1,*String++);
	}

}



void SetCursor(uint8_t x, uint8_t y){
Send(0,0x80+(x+64*y));
}

