#include "LCD.h"
const uint8_t LCD::NbMap[]={0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x6F};


LCD::digit LCD::digits[]={
		{{2,2,2,3,3,3,3},{&LCDDR1,&LCDDR6,&LCDDR11,&LCDDR16,&LCDDR11,&LCDDR1,&LCDDR6}},
		{{0,0,0,1,1,1,1},{&LCDDR1,&LCDDR6,&LCDDR11,&LCDDR16,&LCDDR11,&LCDDR1,&LCDDR6}},
		{{6,6,6,7,7,7,7},{&LCDDR0,&LCDDR5,&LCDDR10,&LCDDR15,&LCDDR10,&LCDDR0,&LCDDR5}},
		{{4,4,4,5,5,5,5},{&LCDDR0,&LCDDR5,&LCDDR10,&LCDDR15,&LCDDR10,&LCDDR0,&LCDDR5}},
		{{2,2,2,3,3,3,3},{&LCDDR0,&LCDDR5,&LCDDR10,&LCDDR15,&LCDDR10,&LCDDR0,&LCDDR5}},
		{{0,0,0,1,1,1,1},{&LCDDR0,&LCDDR5,&LCDDR10,&LCDDR15,&LCDDR10,&LCDDR0,&LCDDR5}}}
;


LCD::LCD(){
	LCDCRB|=(1<<LCDMUX1)|(1<<LCDMUX0);
	LCDFRR|=(1<<LCDPS2)|(1<<LCDCD2); // prescaler 512, division factor 5 & K=8 for duty 1/4
	LCDCCR|=(1<<LCDCC1)|(1<<LCDDC2)|(1<<LCDDC1);
	LCDCRA|=(1<<LCDEN);
}

LCD::~LCD() {
	LCDCRA|=(1<<LCDBL);
	_delay_ms(500);
	LCDCRA&=~(1<<LCDEN);
}

void LCD::setDigit(uint8_t dig, uint8_t nb) {
	for(uint8_t j=0;j<8;j++){
		if(NbMap[nb] & (1<<j)){
	 *(digits[dig].dr[j]) |= (1<<digits[dig].s[j]);
		}else{
	 *(digits[dig].dr[j]) &= ~(1<<digits[dig].s[j]);
		}	
	}/*
	for(uint8_t j=0;j<8;j++){
		*(digits[dig].dr[j]) |= (1<<digits[dig].s[j]);
		_delay_ms(1000);
	}*/

}
/*
void LCD::setDigit(uint8_t dig, Symbol s) {
	for(uint8_t j=0;j<8;j++){
		if(s & (1<<j)){
			digits[dig].dr[j]|=(1<<digits[dig].s[j]);
		}else{
			digits[dig].dr[j]&=~(1<<digits[dig].s[j]);
		}
	}
}*/

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

void LCD::blink(uint16_t t) {
	for(uint16_t i=0;i<t;i++){
		PORTB|=(1<<PB0);
		_delay_ms(15);
		PORTB&=~(1<<PB0);
		_delay_ms(300);
	}
}
