#include "Display.h"

Display::Display(){
	DDRB |=1<<verrou | 1<<horloge | 1<<data;
	//shift();
	TCCR0A|=0<<WGM02|0<<WGM01|0<<WGM00;
	TIMSK|=1<<TOIE0;
}
/*
void Display::shift(void) {
  PORTB &=~(1<<verrou);
  int i;
  for (i = 0; i < 16; i++) {
	PORTB &=~(1<<horloge);
	if( state[i]){
	PORTB |=(1<<data);
	}else{
	PORTB &=~(1<<data);
	}
	PORTB |=(1<<horloge);
  }
	PORTB |=(1<<verrou);
}
void Display::displayNumber(unsigned int number) {
	int i;
	for (i = 0; i<8; i++) {
		nums[i]=number%10;
		number/=10;
	}
}
void Display::displaySingle(unsigned char d, unsigned int nb) {
	int i;
	for (i = 0; i < 7; i++) {//set to default 
		state[i] = 1;
	}
	for (i = 8; i < 16; i++) {//default
		state[i] = 0;
	}
	state[digits[d]] = 0;

	if (nb >= 8) {//convert decimal to binary in last 4bits
		state[12] = 1;
		nb -= 8;
	}
	if (nb >= 4) {
		state[13] = 1;
		nb -= 4;
	}
	if (nb >= 2) {
		state[14] = 1;
		nb -= 2;
	}
	if (nb >= 1) {
		state[15] = 1;
		nb -= 1;
	}
	shift();
}
*/