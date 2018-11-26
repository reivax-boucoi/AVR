#include "LCD.h"
const uint8_t LCD::NbMap[]={0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x6F}; //DP,G,F,E,D,C,B,A
const uint16_t LCD::CharMap[]={0x003F,0x0006,0x045B,0x044F,0x0466,0x046D,0x047D,0x0007,0x047F,0x046F,
		0x0477,0x250F,0x0071,0x210F,0x0079,0x0071,0x043D,0x0476,0x2100,0x0017,0x2B00,0x0038,0x02B6,0x08B6,0x045C,0x0473,0x083F,0x0C73,0x0879,0x2101,0x003E,0x1230,0x1836,0x1A80,0x1280,0x1209,
		0x0A00,0x1080,0x1800,0x0008,0x0200,0x0463,0x0448,0x3F00,0x0000};//CLK,DP,P,Q,N,M,K,J,H,G,F,E,D,C,B,A

Bargraph::Bargraph bargraphMode=Bargraph::OFF;

LCD::dSmall_t LCD::dSmall[]={//TODO pin
		//		DP			G			F				E			D			C			B				A
		{{{6,&LCDDR05},{5,&LCDDR07},{5,&LCDDR17},{4,&LCDDR03},{4,&LCDDR08},{6,&LCDDR00},{7,&LCDDR07},{7,&LCDDR17}}},//Leftmost digit, segs DP-A
		{{{7,&LCDDR05},{1,&LCDDR08},{1,&LCDDR18},{0,&LCDDR01},{0,&LCDDR06},{7,&LCDDR00},{4,&LCDDR08},{4,&LCDDR18}}},
		{{{0,&LCDDR00},{1,&LCDDR09},{0,&LCDDR00},{2,&LCDDR01},{1,&LCDDR06},{1,&LCDDR01},{1,&LCDDR19},{6,&LCDDR18}}},
		{{{0,&LCDDR00},{0,&LCDDR00},{3,&LCDDR09},{4,&LCDDR01},{3,&LCDDR06},{3,&LCDDR01},{0,&LCDDR00},{3,&LCDDR19}}}//Rightmost digit, segs DP-A
};
LCD::dBig_t LCD::dBig[]={//TODO pin
			//CLK		DP		    	P				Q			N			M			K				J			H			G			F			E				D			C			B				A
		{{{0,&LCDDR00},{0,&LCDDR00},{5,&LCDDR19},{5,&LCDDR09},{6,&LCDDR19},{6,&LCDDR09},{7,&LCDDR01},{0,&LCDDR00},{1,&LCDDR07},{1,&LCDDR02},{2,&LCDDR02},{7,&LCDDR09},{4,&LCDDR19},{3,&LCDDR09},{0,&LCDDR02},{0,&LCDDR07}}},//Rightmost digit, segs CLK-A
		{{{0,&LCDDR00},{7,&LCDDR19},{0,&LCDDR19},{0,&LCDDR09},{0,&LCDDR00},{0,&LCDDR00},{4,&LCDDR02},{4,&LCDDR07},{5,&LCDDR07},{5,&LCDDR02},{6,&LCDDR02},{7,&LCDDR08},{2,&LCDDR19},{0,&LCDDR00},{3,&LCDDR02},{3,&LCDDR07}}},
		{{{6,&LCDDR07},{6,&LCDDR18},{0,&LCDDR00},{0,&LCDDR00},{5,&LCDDR18},{0,&LCDDR00},{7,&LCDDR02},{7,&LCDDR07},{0,&LCDDR00},{0,&LCDDR00},{1,&LCDDR03},{0,&LCDDR08},{3,&LCDDR18},{3,&LCDDR08},{2,&LCDDR03},{2,&LCDDR08}}},
		{{{1,&LCDDR08},{0,&LCDDR18},{7,&LCDDR16},{7,&LCDDR06},{6,&LCDDR17},{6,&LCDDR07},{3,&LCDDR03},{3,&LCDDR08},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{5,&LCDDR06},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}}},
		{{{0,&LCDDR00},{5,&LCDDR16},{1,&LCDDR16},{1,&LCDDR06},{6,&LCDDR16},{6,&LCDDR06},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{7,&LCDDR05},{3,&LCDDR16},{3,&LCDDR06},{0,&LCDDR00},{0,&LCDDR00}}},
		{{{0,&LCDDR00},{7,&LCDDR15},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR05},{0,&LCDDR00},{0,&LCDDR00},{4,&LCDDR05},{0,&LCDDR00},{0,&LCDDR00}}},
		{{{0,&LCDDR00},{0,&LCDDR15},{0,&LCDDR00},{3,&LCDDR05},{1,&LCDDR15},{1,&LCDDR05},{0,&LCDDR00},{7,&LCDDR06},{1,&LCDDR05},{1,&LCDDR00},{0,&LCDDR00},{5,&LCDDR05},{4,&LCDDR15},{0,&LCDDR00},{0,&LCDDR00},{0,&LCDDR00}}}//Leftmost digit, segs CLK-A
};
LCD::seg LCD::arrow[]={
		{3,&LCDDR17},{4,&LCDDR00},{3,&LCDDR07},{4,&LCDDR05}//LEFT, RIGHT,UP,DOWN
};
LCD::seg LCD::battery[]={//TODO pin
		{1,&LCDDR17},{5,&LCDDR05},{5,&LCDDR00},{1,&LCDDR07}//Both ends on battery[0]
};
LCD::seg LCD::bargraph[]={//TODO pin
		{6,&LCDDR06},{2,&LCDDR17},//Ends
		{6,&LCDDR01},{2,&LCDDR06},{2,&LCDDR16},{4,&LCDDR16},{0,&LCDDR00},{5,&LCDDR01},{5,&LCDDR06},{2,&LCDDR05},{2,&LCDDR00},{0,&LCDDR00}//Internal
};

LCD::LCD(){
	LCDCRB|=(1<<LCDMUX0)|(1<<LCDMUX1)|(1<<LCDPM3)|(1<<LCDPM2)|(1<<LCDPM1)|(1<<LCDPM0);
	LCDFRR|=(1<<LCDPS0)|(1<<LCDPS1)|(1<<LCDPS2);
	LCDFRR|=(1<<LCDCD2)|(1<<LCDCD1)|(1<<LCDCD0);//16Mhz/(4096*8*8)=61Hz
	//8MHz/(4096*8*5)=48.8Hz
	LCDCCR|=(1<<LCDCC3);
	LCDCRA|=(1<<LCDEN);
}

LCD::~LCD() {
	LCDCRA|=(1<<LCDBL);
	_delay_ms(500);
	LCDCRA&=~(1<<LCDEN);
}

void LCD::setDigit(uint8_t dig, uint8_t nb, bool display) {
	if(display==DSMALL){
		for(uint8_t j=0;j<8;j++){//avoid DP
			if(NbMap[nb] & (1<<(7-j))){
				*(dSmall[dig].segs[j].dr) |= (1<<dSmall[dig].segs[j].s);
			}else{
				*(dSmall[dig].segs[j].dr) &= ~(1<<dSmall[dig].segs[j].s);
			}
		}
	}else{
		for(uint8_t j=1;j<16;j++){	//alphanumeric, avoid DP
			if(CharMap[nb] & (1<<j)){
				*(dBig[dig].segs[j].dr) |= (1<<dBig[dig].segs[j].s);
			}else{
				*(dBig[dig].segs[j].dr) &= ~(1<<dBig[dig].segs[j].s);
			}
		}
	}
}

void LCD::setDigit(uint8_t dig, Symbol sy) {// only available on small display
	for(uint8_t j=0;j<8;j++){
		if(sy & (1<<(7-j))){
			*(dSmall[dig].segs[j].dr) |= (1<<dSmall[dig].segs[j].s);
		}else{
			*(dSmall[dig].segs[j].dr) &= ~(1<<dSmall[dig].segs[j].s);
		}
	}
}

uint8_t LCD::setNb(int32_t nb, bool display) {// TODO 10ms !
	if((display==DBIG && (nb>9999999 || nb <9999999)) || (display==DSMALL && (nb>9999 || nb<-999))){
		return 1;
	}
	int8_t d;
	if(display==DSMALL){
		d=3;
	}else{
		d=6;
	}
	uint8_t min=0;
	if(nb<0){
		min++;
		nb=(~nb)+1;
		if(display == DSMALL){
			setDigit(0,Minus);
		}else{
			setMinus(true);
			setPlus(false);
		}
	}else{
		if(display == DBIG){
			setMinus(false);
			setPlus(true);
		}
	}
while(d>=min){
	setDigit(d--,nb%10,display);
	nb/=10;
	if(nb==0)break;
}
while(d>=min){
	if(display==DSMALL){
		setDigit(d--,Blank);
	}else{
		setDigit(d--,CharMap[44],DBIG);
	}
}
return 0;
}

void LCD::setBattery(Battery::Battery b) {
	uint8_t i=0;
	bool m=false;
	switch(b){
	case Battery::NONE:
		i=0;
		break;
	case Battery::EMPTY:
		i=1;
		break;
	case Battery::FIRST:
		i=2;
		break;
	case Battery::SECOND:
		i=3;
		break;
	case Battery::THIRD:
		i=4;
		break;
	case Battery::ONEBAR:
		i=2;
		m=true;
		break;
	case Battery::TWOBAR:
		i=3;
		m=true;
		break;
	case Battery::FULL:
		i=4;
		m=true;
		break;
	default :
		//Mayday
		break;
	}
	for(uint8_t j=0;j<4;j++){
		if((m && (j<i)) || ((!m) && (j+1)==i)){
			*(battery[j].dr) |= (1<<battery[j].s);
		}else{
			*(battery[j].dr) &= ~(1<<battery[j].s);
		}
	}
}

void LCD::setBargraphMode(Bargraph::Bargraph b) {//TODO fix undefined reference
	//	bargraphMode=b;
	if(b==Bargraph::OFF)setBargraphLevel(0);
}

void LCD::setBargraphLevel(uint8_t l) {//TODO fix undefined reference
	if(l>10)return;
	//	switch(bargraphMode){
	//	case SINGLE:
	for(uint8_t i=0;i<12;i++){//range 0-9
		if(i==(l+2)){
			*(bargraph[i].dr) |= (1<<bargraph[i].s);
		}else{
			*(bargraph[i].dr) &= ~(1<<bargraph[i].s);
		}
	}
	/*	break;
	case STACK:
		for(uint8_t i=0;i<12;i++){
			if(i<(l+2)){
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
	}*/
}

void LCD::setArrow(Arrow::Arrow a, bool b) {
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
	setBargraphMode(Bargraph::OFF);
	setBattery(Battery::NONE);
}

void LCD::setMinus(bool b) {//r15-5
	if(b){
		LCDDR00|=(1<<1);
	}else{
		LCDDR00&=~(1<<1);
	}
}
void LCD::setPlus(bool b) {
	if(b){
		LCDDR00|=(1<<0);
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
}
 */
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
