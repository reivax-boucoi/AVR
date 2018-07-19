#ifndef LCD_H_
#define LCD_H_
#include <avr/io.h>
#include <util/delay.h>

#define DBIG false
#define DSMALL true

class LCD {
public:
	LCD();
	~LCD();
	enum Symbol {A=0x77,b=0x7C,C=0x39,d=0x5E,E=0x79,F=0x71,H=0x76,L=0x38,P=0x73,U=0x3E,
		Minus=0x40,Blank=0x00,Equal=0x48,Degree=0xC3};
	enum Battery {NONE,EMPTY,FIRST,SECOND,THIRD,ONEBAR,TWOBAR,FULL};
	enum Arrow {



	static void setDigit(uint8_t dig, Symbol s, bool display);
	static void setDigit(uint8_t dig, uint8_t nb, bool display);
	static void setBattery(Battery b);
	static void setMinus(bool b);
	static void setPlus(bool b);
	static_

	static bool getMinus(void);
	static bool getPlus(void);
	static uint8_t setNb(int32_t nb, bool display);
	static void clear(void);
private:
	struct display{
		uint8_t s[7];
		volatile uint8_t* dr[7];
	};
	static display dSmall[4];
	static display dBig[7];
	static const uint8_t NbMap[10];

};

#endif
