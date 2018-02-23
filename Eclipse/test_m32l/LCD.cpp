/*
 * LCD.cpp
 *
 *  Created on: 18 f�vr. 2018
 *      Author: User
 */

#include "LCD.h"

LCD::LCD() {

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
