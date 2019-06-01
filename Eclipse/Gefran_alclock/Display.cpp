#include "Display.h"

Display::Display() {

	TCNT0=0;
	OCR0A=0x48;
	TCCR0A|=(1<<WGM01);
	TCCR0B |=(1<<CS02);
	TIMSK0|=(1<<OCIE0A);

	COMREG = 0x1F;
	COMREG1 = 0x3C;
	SEGREG = 0xFF;
	COMPORT=0x1F;
	COMPORT1=0x3C;
	SEGPORT=0x00;
	comcnt=0;
}

void Display::setNum(uint16_t nb, bool display, bool right) {
	setDigit((right<<1)+(display<<2)+1,nb%10+'0');
	setDigit((right<<1)+(display<<2),(nb/10)%10+'0');
	if(nb>99){
		setDigit((right<<1)+(display<<2)-1,(nb/100)%10+'0');
		setDigit((right<<1)+(display<<2)-2,(nb/1000)%10+'0');
	}
}

void Display::setDP(uint8_t digit, bool state){
	displayData[digit+1]=state<<7 | (displayData[digit+1] & 0x7F);
}

void Display::setDigit(uint8_t digit, uint8_t c){
	if(digit<8 && digit>=0){
		if(c>='0' && c<='9'){
			c=NbMap[c-'0'];
		}else if(c>='a' && c<='z'){
			c=LetterMap[c-'a'];
		}else if(c>='A' && c<='Z'){
			c=LetterMap[c-'A'];
		}else if (c=='-') {
			c=1<<4;
		}else{
			c=0;
		}
		displayData[digit+1]=c|(displayData[digit+1] & 1<<7);
	}
}

void Display::setLed(uint8_t led, bool state){
	if(led>=0 && led<7){
		if(state){
			displayData[0] |=LedMap[led];
		}else{
			displayData[0] &=~LedMap[led];
		}
	}
}

void Display::clearDigit(uint8_t digit){
	displayData[digit+1]=0x00;;
}
void Display::clearLeds(void){
	displayData[0]=0x00;
}

void Display::setText(const char* s, uint8_t len, uint8_t display){
	if(len<=4){
		for(uint8_t i=0;i<len;i++){
			setDigit(i+(display<<2),s[i]);
		}
	}
}

void Display::muxISR(void){
	if(comcnt<3 || comcnt == 4){
		COMPORT1 =regMap[comcnt];
		COMPORT |=0x1F;
	}else{
		COMPORT =regMap[comcnt];
		COMPORT1 |=0x3C;
	}
	SEGPORT=displayData[comcnt];

	comcnt++;
	if(comcnt>=MAXCOMCNT)comcnt=0;


}
Display::~Display() {

}

