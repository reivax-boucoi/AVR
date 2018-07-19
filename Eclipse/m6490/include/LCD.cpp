#include "LCD.h"

const uint8_t LCD::NbMap[]={0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x6F}; //DP,G,F,E,D,C,B,A
LCD::Bargraph bargraphMode=LCD::OFF;
LCD::dSmall_t LCD::dSmall[]={//TODO pin & fix
		{{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}}},
		{{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}}},
		{{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}}},
		{{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}}}
};
LCD::dBig_t LCD::dBig[]={//TODO pin & fix
		{{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}}},
		{{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}}},
		{{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}}},
		{{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}}},
		{{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}}},
		{{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}}},
		{{{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}}}
};
LCD::seg LCD::arrow[]={//TODO pin
		{0,&LCDDR00},
		{0,&LCDDR00},
		{0,&LCDDR00},
		{0,&LCDDR00}
};
LCD::seg LCD::battery[]={//TODO pin
		{0,&LCDDR00},
		{0,&LCDDR00},
		{0,&LCDDR00},
		{0,&LCDDR00}
};
LCD::seg LCD::bargraph[]={//TODO pin
		{0,&LCDDR00},
		{0,&LCDDR00},
		{0,&LCDDR00},
		{0,&LCDDR00},
		{0,&LCDDR00},
		{0,&LCDDR00},
		{0,&LCDDR00},
		{0,&LCDDR00},
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

void LCD::setBargraphLevel(uint8_t l) {
	if(l>10)return;
	switch(bargraphMode){
	case SINGLE:
		for(uint8_t i=0;i<12;i++){
			if(i==(l+2)){//TODO check offset
				*(bargraph[i].dr) |= (1<<bargraph[i].s);
			}else{
				*(bargraph[i].dr) &= ~(1<<bargraph[i].s);
			}
		}
		break;
	case STACK:
		for(uint8_t i=0;i<12;i++){
			if(i<(l+2)){ // TODO check offset
				*(bargraph[i].dr) |= (1<<bargraph[i].s);
			}else{
				*(bargraph[i].dr) &= ~(1<<bargraph[i].s);
			}
		}
		break;
	case OFF:
	default:
		for(uint8_t i=0;i<12;i++){
			*(bargraph[i].dr) &= ~(1<<bargraph[i].s);
		}
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
	clearArrows();
	clearDisplay(DBIG);
	clearDisplay(DSMALL);
}

void LCD::setMinus(bool b) {//TODO pin
}

void LCD::setPlus(bool b) {//TODO pin
	if(b){
		LCDDR00|=1<<0;
	}else{
		LCDDR00&=~(1<<0);
}
}

bool LCD::getMinus(void) {//TODO pin
	return false;
}

bool LCD::getPlus(void) {//TODO pin
	return false;
}

void LCD::clearArrows(void) {
	for(uint8_t i=0;i<4;i++){
		*(arrow[i].dr) &= ~(1<<arrow[i].s);
	}
}
/*
uint8_t LCD::setNb(float nb, bool display) {//TODO
	return 0;
}*/

uint8_t LCD::setNb(char* str[]) {//TODO
	return 0;
}

void LCD::clearDisplay(bool display) {
	if(display==DBIG){
		for(uint8_t j=0;j<7;j++){
			for(uint8_t i=0;i<15;i++){
				*(dBig[j].segs[i].dr) &= ~(1<<dBig[j].segs[i].s);
			}
		}
	}else{
		for(uint8_t j=0;j<4;j++){
			for(uint8_t i=0;i<8;i++){
				*(dSmall[j].segs[i].dr) &= ~(1<<dSmall[j].segs[i].s);
			}
		}
	}
}
