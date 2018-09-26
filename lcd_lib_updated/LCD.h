/*
15/04/16
(Ajout float 29/7/16)

Library LCD, 8 pin donnees, testee sur ATMEGA328p
Penser à LCDInit();
Résolution Float de 3-4 positions
tester delais init lors chgt crystal
*/


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

char numString[10];

void toggle(void);
void Send(unsigned char state, unsigned char data);

void SendString(char *String);
void SendStringPos(uint8_t x, uint8_t y, char *String);

void SetCursor(uint8_t x, uint8_t y);

void LCDInit(void);

void SendFloatPos(uint8_t x, uint8_t y, float val);
void SendFloat(float val);

void SendInt(int val);
void SendIntPos(uint8_t x, uint8_t y, int val);

#endif