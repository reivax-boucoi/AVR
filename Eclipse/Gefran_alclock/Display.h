#ifndef DISPLAY_H_
#define DISPLAY_H_
#include <avr/io.h>
#define COMPORT PORTB
#define COMREG DDRB
#define COMPORT1 PORTD
#define COMREG1 DDRD
#define SEGPORT PORTA
#define SEGREG DDRA

#define MAXCOMCNT 9
						//0,   1,   2,   3,   4,   5,   6,   7,   8,   9
const uint8_t NbMap[10]={0x6F,0x0C,0x75,0x5D,0x1E,0x5B,0x7B,0x0D,0x7F,0x5F};
						//    A,   B,   C,   D,   E,   F,   G,   H,   I,   J,   K,   L,   M,   N,   O,   P,   Q,   R,   S,   T,   U,   V,   W,   X,   Y,   Z
const uint8_t LetterMap[26]={0x3F,0x7A,0x63,0x7C,0x73,0x33,0x6B,0x3E,0x22,0x6C,0x76,0x62,0x2F,0x38,0x78,0x37,0x1F,0x30,0x5B,0x72,0x6E,0x68,0x46,0x1A,0x5E,0x75};
					   //   A,   B,   C,   D,   E,   F,   G,   DP
const uint8_t segMap[8]={1<<0,1<<2,1<<3,1<<6,1<<5,1<<1,1<<4,1<<7};

							//   LED*, H1*, H2*, H3 , H4*, L1 , L2 , L3 , L4
const uint8_t regMap[MAXCOMCNT]={0x2C,0x38,0x1C,0x1B,0x34,0x0F,0x17,0x1D,0x1E};


				//OUT1,OUT2,OUT3,OUT4,L1,L2,L3
const uint8_t LedMap[7]={0x01,0x04,0x08,0x40,0x20,0x02,0x10};



class Display {
public:
	Display();

	void setNum(uint16_t nb, bool display, bool left);
	void setText(const char* s, uint8_t len, uint8_t display);
	void setDigit(uint8_t digit, uint8_t nb);
	void setDP(uint8_t digit, bool state);
	void setLed(uint8_t led, bool state);
	void clearDigit(uint8_t digit);
	void clearLeds(void);
	
	void muxISR(void);
	
	~Display();

private :
	uint8_t comcnt;
	uint8_t displayData[MAXCOMCNT];
	
};

#endif
