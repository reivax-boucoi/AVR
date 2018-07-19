#include "LCD.h"

const uint8_t LCD::NbMap[]={0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x6F};
LCD::display LCD::dSmall[]={
		{{2,2,2,3,3,3,3},{&LCDDR01,&LCDDR06,&LCDDR11,&LCDDR16,&LCDDR11,&LCDDR01,&LCDDR06}},
		{{0,0,0,1,1,1,1},{&LCDDR01,&LCDDR06,&LCDDR11,&LCDDR16,&LCDDR11,&LCDDR01,&LCDDR06}},
		{{6,6,6,7,7,7,7},{&LCDDR00,&LCDDR05,&LCDDR10,&LCDDR15,&LCDDR10,&LCDDR00,&LCDDR05}},
		{{0,0,0,1,1,1,1},{&LCDDR00,&LCDDR05,&LCDDR10,&LCDDR15,&LCDDR10,&LCDDR00,&LCDDR05}}};
LCD::display LCD::dBig[]={
		{{2,2,2,3,3,3,3},{&LCDDR01,&LCDDR06,&LCDDR11,&LCDDR16,&LCDDR11,&LCDDR01,&LCDDR06}},
		{{0,0,0,1,1,1,1},{&LCDDR01,&LCDDR06,&LCDDR11,&LCDDR16,&LCDDR11,&LCDDR01,&LCDDR06}},
		{{6,6,6,7,7,7,7},{&LCDDR00,&LCDDR05,&LCDDR10,&LCDDR15,&LCDDR10,&LCDDR00,&LCDDR05}},
		{{2,2,2,3,3,3,3},{&LCDDR01,&LCDDR06,&LCDDR11,&LCDDR16,&LCDDR11,&LCDDR01,&LCDDR06}},
		{{0,0,0,1,1,1,1},{&LCDDR01,&LCDDR06,&LCDDR11,&LCDDR16,&LCDDR11,&LCDDR01,&LCDDR06}},
		{{6,6,6,7,7,7,7},{&LCDDR00,&LCDDR05,&LCDDR10,&LCDDR15,&LCDDR10,&LCDDR00,&LCDDR05}},
		{{0,0,0,1,1,1,1},{&LCDDR00,&LCDDR05,&LCDDR10,&LCDDR15,&LCDDR10,&LCDDR00,&LCDDR05}}};

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
				*(dSmall[dig].dr[j]) |= (1<<dSmall[dig].s[j]);
			}else{
				*(dBig[dig].dr[j]) |= (1<<dBig[dig].s[j]);
			}
		}else{
			if(display==DSMALL){
				*(dSmall[dig].dr[j]) &= ~(1<<dSmall[dig].s[j]);
			}else{
				*(dBig[dig].dr[j]) &= ~(1<<dBig[dig].s[j]);
			}
		}
	}
}

void LCD::setDigit(uint8_t dig, Symbol sy, bool display) {
	for(uint8_t j=0;j<8;j++){
		if(sy & (1<<j)){
			if(display==DSMALL){
				*(dSmall[dig].dr[j]) |= (1<<dSmall[dig].s[j]);
			}else{
				*(dBig[dig].dr[j]) |= (1<<dBig[dig].s[j]);
			}
		}else{
			if(display==DSMALL){
				*(dSmall[dig].dr[j]) &= ~(1<<dSmall[dig].s[j]);
			}else{
				*(dBig[dig].dr[j]) &= ~(1<<dBig[dig].s[j]);
			}
		}
	}
}

void LCD::setBattery(Battery b) {
	switch(b){
	case NONE:
		LCDDR1 &=~(1<<4);
		LCDDR6 &=~(1<<4);
		LCDDR11 &=~(1<<4);
		LCDDR16 &=~(1<<4);
		break;
	case EMPTY:
		LCDDR1 |=(1<<4);
		LCDDR6 &=~(1<<4);
		LCDDR11 &=~(1<<4);
		LCDDR16 &=~(1<<4);
		break;
	case FIRST:
		LCDDR1 &=~(1<<4);
		LCDDR6 &=~(1<<4);
		LCDDR11 &=~(1<<4);
		LCDDR16 |=(1<<4);
		break;
	case SECOND:
		LCDDR1 &=~(1<<4);
		LCDDR6 &=~(1<<4);
		LCDDR11 |=(1<<4);
		LCDDR16 &=~(1<<4);
		break;
	case THIRD:
		LCDDR1 &=~(1<<4);
		LCDDR6 |=(1<<4);
		LCDDR11 &=~(1<<4);
		LCDDR16 &=~(1<<4);
		break;
	case ONEBAR:
		LCDDR1 |=(1<<4);
		LCDDR6 &=~(1<<4);
		LCDDR11 &=~(1<<4);
		LCDDR16 |=(1<<4);
		break;
	case TWOBAR:
		LCDDR1 |=(1<<4);
		LCDDR6 &=~(1<<4);
		LCDDR11 |=(1<<4);
		LCDDR16 |=(1<<4);
		break;
	case FULL:
		LCDDR1 |=(1<<4);
		LCDDR6 |=(1<<4);
		LCDDR11 |=(1<<4);
		LCDDR16 |=(1<<4);
		break;
	}
}

void LCD::setDP(bool b) {
	if(b){
		LCDDR15|=(1<<4);
	}else{
		LCDDR15&=~(1<<4);
	}
}

void LCD::setClk(bool b) {
	if(b){
		LCDDR16|=1;
	}else{
		LCDDR16&=~1;
	}
}

bool LCD::getDP(void) {
	return (LCDDR15 & (1<<4));
}

bool LCD::getClk(void) {
	return (LCDDR16 & 1);
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

void LCD::clear(void) {//TODO
}
