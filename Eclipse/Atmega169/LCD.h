#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <util/delay.h>
class LCD {
public:
	enum Symbols {A=0x77,b=0x1F,C=0x4E,d=0x3D,E=0x4F,F=0x47,H=0x37,L=0xE,P=0x67,U=0x3E};
	enum Specials {NONE=0x00,EMPTY=0x20,ONEBAR=0x30,TWOBAR=0x38,FULL=0x3C,CLK=0x2,DOT=0x1};
	LCD();
	~LCD();
	//void setDigit(uint8_t d, symbol s);
	void setDigit(uint8_t d, uint8_t nb);
	void blink(uint8_t t);
};

#endif
