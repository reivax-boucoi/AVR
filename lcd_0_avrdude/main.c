#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define LCDDataPORT PORTB // bus on port B
#define LCDDataDIR DDRB
#define LCDControlPORT PORTD // control on port D
#define LCDControlDIR DDRD
#define Enable 5
#define RW 7
#define RS 2

int numX=0;
int numY=0;
char numString[2];
void toggle(void);
void Send(unsigned char state, unsigned char data);
void SendString(char *String);
void SendStringP(int x, int y, char *String);
void SetCursor(int x, int y);


int main(void){

	LCDControlDIR |= 1<<Enable | 1<<RW | 1<<RS;
	_delay_ms(15);
	Send(0,0x01); //Clear display + set cursor at home pos°
	_delay_ms(2);
	Send(0,0x38); // set interface to 8bits, nb disp lines to 2, disp mode 5*8
	_delay_us(50);
	Send(0,0x0E); // set display on, cursor on, blinking off
	_delay_us(50);
	SendString("x=");
	SendStringP(5,0,"y=");
	
	while(1){
	for(numY=0;numY<2;numY++){
		SendString("x=");
		SendStringP(5,0,"y=");
			for(numX=0;numX<16;numX++){
			
				itoa(numX,numString,10);
				SendStringP(2,0,numString);
				itoa(numY,numString,10);
				SendStringP(7,0,numString);
				SetCursor(numX,numY);
				
				_delay_ms(500);
			}
		Send(0,0x01);
		_delay_ms(2);
		}
	}
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



void SendString(char *String){
	while(*String>0){
		Send(1,*String++);
	}

}
void SendStringP(int x, int y, char *String){
	SetCursor(x,y);
	while(*String>0){
		Send(1,*String++);
	}

}



void SetCursor(int x, int y){
Send(0,0x80+(x+64*y));
}