#include "LCD.h"

const uint8_t LCD::NbMap[]={0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x6F};
LCD::Bargraph bargraphMode=LCD::OFF;/*
LCD::dSmall_t LCD::dSmall[]={//TODO swap struct
		{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}},
		{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}},
		{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}},
		{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}}
};
LCD::dBig_t LCD::dBig[]={//TODO swap struct
		{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}},
		{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}},
		{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}},
		{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}},
		{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}},
		{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}},
		{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}}
};*/
LCD::seg LCD::arrow[]={//TODO pin
		{0,&LCDDR00},
		{0,&LCDDR00},
		{0,&LCDDR00},
		{0,&LCDDR00}
};
LCD::LCD(){
	LCDCRB|=(1<<LCDMUX0)|(1<<LCDMUX1)|(1<<LCDPM3)|(1<<LCDPM2)|(1<<LCDPM1)|(1<<LCDPM0);
	LCDFRR|=(1<<LCDPS0)|(1<<LCDPS1)|(1<<LCDPS2)|(1<<LCDCD2);	//8MHz/(4096*8*5)=48.8Hz
	LCDCCR|=(1<<LCDCC3);
	LCDCRA|=(1<<LCDEN);
}

LCD::~LCD() {
	LCDCRA|=(1<<LCDBL);
	_delay_ms(500);
	LCDCRA&=~(1<<LCDEN);
}

void LCD::setDigit(uint8_t dig, uint8_t nb, bool display) {
	for(uint8_t j=0;j<8;j++){
		if(NbMap[nb] & (1<<j)){
			if(display==DSMALL){
				*(dSmall[dig].segs[j].dr) |= (1<<dSmall[dig].segs[j].s);
			}else{
				*(dBig[dig].segs[j].dr) |= (1<<dBig[dig].segs[j].s);
			}
		}else{
			if(display==DSMALL){
				*(dSmall[dig].segs[j].dr) &= ~(1<<dSmall[dig].segs[j].s);
			}else{
				*(dBig[dig].segs[j].dr) &= ~(1<<dBig[dig].segs[j].s);
			}
		}
	}
}

void LCD::setDigit(uint8_t dig, Symbol sy) {
	for(uint8_t j=0;j<8;j++){
		if(sy & (1<<j)){
			*(dSmall[dig].segs[j].dr) |= (1<<dSmall[dig].segs[j].s);
		}else{
			*(dSmall[dig].segs[j].dr) &= ~(1<<dSmall[dig].segs[j].s);
		}
	}
}

uint8_t LCD::setNb(int32_t nb, bool display) {// TODO 10ms !
	if((DBIG && (nb>9999999 || nb <9999999)) || (DSMALL && (nb>9999 || nb<-999))){
		return 1;
	}
	int8_t d;
	if(display==DSMALL){
		d=3;
	}else{
		d=6;
	}
	uint8_t min=0;
	if(nb<0 && display == DSMALL){
		setDigit(0,Minus,display);
		min++;
		nb=(~nb)+1;
	}else if(display == DBIG){
		if(nb<0){
			setMinus(true);
			setPlus(false);
		}else{
			setMinus(false);
			setPlus(true);
		}
	}
	while(d>=min){
		setDigit(d--,nb%10,display);
		nb/=10;
		if(nb==0)break;
	}
	while(d>=min)setDigit(d--,Blank,display);
	return 0;
}

void LCD::setBattery(Battery b) {
}

void LCD::setBargraphMode(Bargraph b) {
	bargraphMode=b;
}

void LCD::setBargraphLevel(uint8_t l) {//TODO pin
	switch(bargraphMode){
	case SINGLE:

		break;
	case STACK:

		break;
	case OFF:
	default:

		break;
	}
}

void LCD::setArrow(Arrow a, bool b) {
	if(b){
		*(arrow[a].dr) |= (1<<arrow[a].s);
	}else{
		*(arrow[a].dr) &= ~(1<<arrow[a].s);
	}
}

void LCD::clear(void) {//TODO
}

void LCD::setMinus(bool b) {
}

void LCD::setPlus(bool b) {
}

bool LCD::getMinus(void) {
	return false;
}

bool LCD::getPlus(void) {
	return false;
}

void LCD::clearArrows(void) {
}

void LCD::clearDisplay(bool display) {
}
