#ifndef LCD
#define LCD


#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define LCDDataPORT PORTB // bus on port B
#define LCDDataDIR DDRB
#define LCDControlPORT PORTC // control on port D
#define LCDControlDIR DDRC
#define Enable 0
#define RW 1
#define RS 2

void toggle(void);
void Send(unsigned char state, unsigned char data);
void SendString(char *String);
void SendStringPos(int x, int y, char *String);
void SetCursor(uint8_t x, uint8_t y);
void LCDInit(void);



#endif