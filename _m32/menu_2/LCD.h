#ifndef LCD
#define LCD


#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define LCDDataPORT PORTC // bus on port B
#define LCDDataDIR DDRC
#define LCDControlPORT PORTD // control on port D
#define LCDControlDIR DDRD
#define Enable 5
#define RW 7
#define RS 6

void toggle(void);
void Send(unsigned char state, unsigned char data);
void SendString(char *String);
void SendStringPos(int x, int y, char *String);
void SetCursor(uint8_t x, uint8_t y);
void LCDInit(void);



#endif