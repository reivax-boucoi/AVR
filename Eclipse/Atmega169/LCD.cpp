#include "LCD.h"

LCD::LCD(){
	LCDCRB|=(1<<LCDMUX1)|(1<<LCDMUX0);
	LCDFRR|=(1<<LCDPS2)|(1<<LCDCD2); // prescaler 512, division factor 5 & K=8 for duty 1/4
	LCDCCR|=(1<<LCDCC1)|(1<<LCDDC2)|(1<<LCDDC1);
	LCDCRA|=(1<<LCDEN);
	digits[0].dr={0,LCDDR6,LCDDR11,LCDDR16,LCDDR11,0,LCDDR6};
	digits[1].dr={0,LCDDR6,LCDDR11,LCDDR16,LCDDR11,0,LCDDR6};
	digits[2].dr={0,LCDDR5,LCDDR10,LCDDR15,LCDDR10,0,LCDDR5};
	digits[3].dr={0,LCDDR5,LCDDR10,LCDDR15,LCDDR10,0,LCDDR5};
	digits[4].dr={0,LCDDR5,LCDDR10,LCDDR15,LCDDR10,0,LCDDR5};
	digits[5].dr={0,LCDDR5,LCDDR10,LCDDR15,LCDDR10,0,LCDDR5};

	digits[0]={0,3,2,3,2,0,3};
	digits[1]={0,1,0,1,0,0,1};
	digits[2]={0,7,6,7,6,0,7};
	digits[3]={0,5,4,5,4,0,5};
	digits[4]={0,3,2,3,2,0,3};
	digits[5]={0,1,0,1,0,0,1};

}

LCD::~LCD() {
	LCDCRA|=(1<<LCDBL);
	_delay_ms(500);
	LCDCRA&=~(1<<LCDEN);
}
void LCD::setDigit(uint8_t dig, uint8_t nb) {
	for(uint8_t j=0;j<8;j++){
		if(NbMap[nb] & (1<<j)){
			digits[dig].dr[j]|=(1<<digits[dig].s[j])
		}else{
			digits[dig].dr[j]&=~(1<<digits[dig].s[j]);
		}
	}
}

void LCD::setDigit(uint8_t dig, Symbol s) {
	for(uint8_t j=0;j<8;j++){
		if(s & (1<<j)){
			digits[dig].dr[j]|=(1<<digits[dig].s[j])
		}else{
			digits[dig].dr[j]&=~(1<<digits[dig].s[j]);
		}
	}
}

void LCD::setBattery(Battery b) {
	switch(b){//TODO add T1 segment here ! (T1off when not specified
	case NONE:
		LCDDR6 &=~(1<<4);
		LCDDR11 &=~(1<<4);
		LCDDR16 &=~(1<<4);
		break;
	case EMPTY:
		LCDDR6 &=~(1<<4);
		LCDDR11 &=~(1<<4);
		LCDDR16 &=~(1<<4);
		//T1 on
		break;
	case FIRST:
		LCDDR6 &=~(1<<4);
		LCDDR11 &=~(1<<4);
		LCDDR16 |=(1<<4);
		break;
	case SECOND:
		LCDDR6 &=~(1<<4);
		LCDDR11 |=(1<<4);
		LCDDR16 &=~(1<<4);
		break;
	case THIRD:
		LCDDR6 |=(1<<4);
		LCDDR11 &=~(1<<4);
		LCDDR16 &=~(1<<4);
		break;
	case ONEBAR:
		LCDDR6 &=~(1<<4);
		LCDDR11 &=~(1<<4);
		LCDDR16 |=(1<<4);
		//t1on
		break;
	case TWOBAR:
		LCDDR6 &=~(1<<4);
		LCDDR11 |=(1<<4);
		LCDDR16 |=(1<<4);
		//t1on
		break;
	case FULL:
		LCDDR6 |=(1<<4);
		LCDDR11 |=(1<<4);
		LCDDR16 |=(1<<4);
		//t1on
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

void LCD::blink(uint8_t t) {
	for(int i=0;i<t;i++){
		PORTB|=(1<<PB0);
		_delay_ms(15);
		PORTB&=~(1<<PB0);
		_delay_ms(300);
	}
}
