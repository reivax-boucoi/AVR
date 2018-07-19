#ifndef LCD_H_
#define LCD_H_
#include <avr/io.h>
#include <util/delay.h>


/*Atmega6490 is missing 2 segment pins :
On display
	pin 36 is battery
	pin 35 is arrows
	pin 42 is µmHF
*/
#define DBIG false
#define DSMALL true

class LCD {
public:
	LCD();
	~LCD();
	enum Symbol {A=0x77,b=0x7C,C=0x39,d=0x5E,E=0x79,F=0x71,H=0x76,L=0x38,P=0x73,U=0x3E,
		Minus=0x40,Blank=0x00,Equal=0x48,Degree=0xC3};
	enum Battery {NONE,EMPTY,FIRST,SECOND,THIRD,ONEBAR,TWOBAR,FULL};
	enum Arrow {LEFT,RIGHT,UP,DOWN};
	enum Bargraph {OFF,STACK,SINGLE};


//high level
	static uint8_t setNb(int32_t nb, bool display);
	static uint8_t setNb(float nb, bool display);
	static uint8_t setNb(char* str[]);//only available on big display
	static void clear(void);

//mid level
	static void setDigit(uint8_t dig, Symbol s);// only available on small display
	static void setDigit(uint8_t dig, uint8_t nb, bool display);
	static void setBattery(Battery b);
	static void setBargraphMode(Bargraph b);
	static void setBargraphLevel(uint8_t l);
	static void setMinus(bool b);
	static void setPlus(bool b);
	static void setArrow(Arrow a, bool b);

	static bool getMinus(void);
	static bool getPlus(void);

	static void clearArrows(void);
	static void clearDisplay(bool display);

private:

	struct seg{
		uint8_t s;
		volatile uint8_t* dr;
	};
	struct dSmall_t{
		static seg segs[8];//classic 7 segment+DP
	};
	struct dBig_t{//extended alphanumeric segments+DP
		static seg segs[15];
	};

	static dSmall_t dSmall[4]; // 4 7seg digits
	static dBig_t dBig[7]; // 7 alphanumeric digits
	static seg arrow[4];
	static seg bargraph[12];
	static seg battery[4];
	static const uint8_t NbMap[10];
	static Bargraph bargraphMode;
};

#endif
