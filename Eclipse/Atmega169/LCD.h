#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <util/delay.h>
class LCD {
public:
	static const uint8_t NbMap[10]={0x7E,0x30,0x6D,0x79,0x33,0x5B,0x5F,0x70,0x7F,0x7B};
	enum Symbol {A=0x77,b=0x1F,C=0x4E,d=0x3D,E=0x4F,F=0x47,H=0x37,L=0xE,P=0x67,U=0x3E};

	enum Battery {NONE,EMPTY,FIRST,SECOND,THIRD,ONEBAR,TWOBAR,FULL};

	struct digit{
		uint8_t s[7];
		uint8_t dr[7];
	}digits[6];

	LCD();
	~LCD();
	void setDigit(uint8_t dig, Symbol s);
	void setDigit(uint8_t dig, uint8_t nb);
	void setBattery(Battery b);
	void setDP(bool b);
	void setClk(bool b);
	void blink(uint8_t t);
};

#endif
