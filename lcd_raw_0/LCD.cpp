/*
 * LCD.cpp
 *
 *  Created on: 18 f�vr. 2018
 *      Author: User
 */

#include "LCD.h"

LCD::LCD() {
	spec = NONE;
	state=0;
	SEGREG0 = 0xFF;
	SEGREG1 |= 0x80;
	COMREG |= 0x78;
	//SEGPORT0 = 0x00;
	SEGPORT0 = 0xFF;
	//SEGPORT1 &= ~0x80;
	SEGPORT1 |= 0x80;
	COMPORT &= ~0x78;
	OCR0=63;
	TCCR0 |= (1<<WGM01)|(1<<CS02)|(1<<CS00);
	TIMSK|=(1<<OCIE0);

}

LCD::~LCD() {
}


int8_t LCD::DisplayNB(float nb) {
return 0;
}

int8_t LCD::DisplayNB(int32_t nb) {
	if(nb>999999)return -1;
	if(nb<-99999)return -2;
	if(nb>=0){
		for(int8_t i=5;i>=0;i--){
			//digits[i]=nb/
		}
	}else{

	}
	return 0;
}

Specials LCD::getSpec() const {	return spec;}

void LCD::setSpec(Specials spec) {	this->spec = spec;}

const Symbols LCD::getDigits(uint8_t d) const {	return digits[d];}

void LCD::Blink() {
	PORTB^=1<<PINB0;
}

void LCD::timer() {
	switch(++state){
		case 1:

		break;
		case 2:

		break;
		case 3:
			//COMPORT
			state=0;
		break;
		default:
		//HELP!
		break;
	}
	SEGPORT0^=0xFF;
	SEGPORT1^=0x80;
	COMPORT^=0x78;
}
