#ifndef DISPLAY_H_
#define DISPLAY_H_
#include <avr/io.h>

#define DISPLAYREG DDRA
#define DISPLAYPORT PORTA
#define CLK PORTA2  // to CLK
#define DATA  PORTA0  // to CS
#define LATCH  PORTA1 // to DATA

#define DECODE  0x09 // regs
#define INTENSITY 0x0A
#define SCAN_LIMIT  0x0B
#define POWER 0x0C
#define TEST  0x0F
#define LEDS  0x07
#define D0  0x06
#define D1  0x05
#define D2  0x04
#define D3  0x03
#define D4  0x02
#define D5  0x01

#define BLANK 0xF // segs
#define DASH  0xA
#define LH  0xC //00001100
#define LE  0xB
#define LL  0xD
#define LP  0xE
#define MHZ 0x20  //B
#define KHZ 0x40  //A
#define HZ  0x10  //C

#define NBDIGITS 6

void initDisplay(void);
void sendByte (uint8_t databyte);
void setRange(uint8_t range);
void writeData(uint8_t reg, uint8_t data);
void clearDisplay();
void displayNumberDP(uint32_t number, uint8_t dp_digit);
void displayNumber(uint32_t number);
#endif /* DISPLAY_H_ */
