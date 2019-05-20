#include "Display.h"

Display::Display() {
	COMREG = 0xFF;
	COMREG1 = 0xFF;
	SEGREG = 0xFF;
	COMPORT=0x00;
	COMPORT1=0x00;
	SEGPORT=0x00;
	comcnt=0;
}

void Display::setNum1(uint16_t nb) {

}

void Display::setDigit(uint8_t digit, uint8_t nb){
	if(nb>=0 && nb<10 && digit<8 && digit>=0){
		//uint8_t value=NbMap[nb];
		//displayData[regMap[digit]]=segMap[NbMap[nb]];
	}
}

void Display::setLed(uint8_t led, bool state){
	if(led>=0 && led<7){
		if(state){
		displayData[regMap[MAXCOMCNT-1]] |=(1<<LedMap[led]);
		}else{
		displayData[regMap[MAXCOMCNT-1]] &=~(1<<LedMap[led]);
		}
	}
}

void Display::muxISR(void){
	
	if(comcnt>7){
		COMPORT1 &=~(1<<comcnt);
	}else{
		COMPORT &=~(1<<comcnt);
	}
	
	comcnt++;
	if(comcnt>=MAXCOMCNT)comcnt=0;
	
	SEGREG=displayData[comcnt];
	
	if(comcnt>7){
		COMPORT1 |=(1<<comcnt);		
	}else{
		COMPORT |=(1<<comcnt);		
	}
}
Display::~Display() {
	
}

