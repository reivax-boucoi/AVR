#ifndef LCD_H_
#define LCD_H_
#include <avr/io.h>
#include <util/delay.h>


/*Atmega6490 is missing 2 segment pins :
 * bargraph >
 * bargraph <
 * µ R01-0
 * H R06-0
 * colon dsmall r3-6
 * olimex r04-7

 */
#define DBIG false
#define DSMALL true
namespace Battery{
enum Battery {NONE,EMPTY,FIRST,SECOND,THIRD,ONEBAR,TWOBAR,FULL};
}
namespace Arrow{
enum Arrow {LEFT,RIGHT,UP,DOWN};
}/*
namespace Bargraph{
enum Bargraph {OFF,STACK,SINGLE};
}*/
class LCD {
public:
	LCD();
	~LCD();
	enum Symbol {A=0x77,b=0x7C,C=0x39,d=0x5E,E=0x79,F=0x71,H=0x76,L=0x38,P=0x73,U=0x3E,
		Minus=0x40,Blank=0x00,Equal=0x48,Degree=0xC3};


	//high level
	static uint8_t setNb(int32_t nb, bool display);
	//static uint8_t setNb(float nb, bool display);
	static uint8_t setNb(char* str[]);//only available on big display
	static void clear(void);

	//mid level
	static void setDigit(uint8_t dig, Symbol s);// only available on small display
	static void setDigit(uint8_t dig, uint8_t nb, bool display);
	static void setBattery(Battery::Battery b);
	//static void setBargraphMode(Bargraph::Bargraph b);
	//static void setBargraphLevel(uint8_t l);
	static void setMinus(bool b);
	static void setPlus(bool b);
	static void setArrow(Arrow::Arrow a, bool b);

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
		seg segs[8];//classic 7 segment+DP
	};
	struct dBig_t{//extended alphanumeric segments+DP
		seg segs[16];
	};

	static dSmall_t dSmall[4]; // 4 7seg digits
	static dBig_t dBig[7]; // 7 alphanumeric digits
	static seg arrow[4];
	static seg bargraph[12];
	static seg battery[4];
	static const uint8_t NbMap[10];//0-9
	static const uint16_t CharMap[45];//0-9,A-Z,<,>,_,',°,=,BLANK
	//static Bargraph::Bargraph bargraphMode;
};
#endif
