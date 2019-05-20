#ifndef DISPLAY_H_
#define DISPLAY_H_
#include <avr/io.h>
#define COMPORT PORTB
#define COMREG DDRB
#define COMPORT1 PORTA
#define COMREG1 DDRA
#define SEGPORT PORTC
#define SEGREG DDRC

#define MAXCOMCNT 9
						//0,   1,   2,   3,   4,   5,   6,   7,   8,   9
const uint8_t NbMap[18]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						//A,   C,   E,   F,   H,   L,   P,   U
						 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
					   //A,B,C,D,E,F,G,DP
const uint8_t segMap[8]={0,1,2,3,4,5,6,7};

				//H1,H2,H3,H4,L1,L2,L3,L4,LEDS
const uint8_t regMap[MAXCOMCNT]={0,1,2,3,4,5,6,7,8};


				//OUT1,OUT2,OUT3,OUT4,L1,L2,L3
const uint8_t LedMap[7]={0,1,2,3,4,5,6};



class Display {
public:
	Display();
	
	void setNum1(uint16_t nb);
	void setDigit(uint8_t digit, uint8_t nb);
	void setLed(uint8_t led, bool state);
	
	void muxISR(void);
	uint8_t displayData[MAXCOMCNT];
	
	~Display();

private :
	uint8_t comcnt;
	
};

#endif
