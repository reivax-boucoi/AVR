#ifndef DEFINES_H
#define DEFINES_H
#include <avr/io.h>

#define CLK    (1<<PA3) //pin 15, store on rising edge
#define DATA   (1<<PA1) //pin 27
#define STROBE (1<<PA2) //pin 16 latch on rising edge
#define LED    (1<<PA0) 
#define BTNSELECT (1<<PB2)
#define BTNOK (1<<PA7)
#define BTNINTERNAL (1<<PA5)

#define F_I2C 400000UL
#define RTC_addr 0x68
#define I2CREAD  0x01
#define I2CWRITE 0x00
#define I2C_FAST_MODE 0
#define I2C_TLOW	1
#define I2C_THIGH	0.7
#define DDR_USI             DDRA
#define PORT_USI            PORTA
#define PIN_USI             PINA
#define PORT_USI_SDA        PA6
#define PORT_USI_SCL        PA4
#define PIN_USI_SDA         PINA6
#define PIN_USI_SCL         PINA4

#define NBLEDS 19
#define DELLONG

#define G1   0b10000000000000000000
#define G2   0b01000000000000000000
#define B1   0b00100000000000000000
#define B2   0b00010000000000000000
#define R1   0b00001000000000000000
#define R2   0b00000100000000000000
#define LEDR 0b00000000100000000000
#define LEDB 0b00000000010000000000

enum Colors{BLACK=0b00000000,RED=0b00000100,GREEN=0b00000010,BLUE=0b00000001,CYAN=GREEN+BLUE,MAGENTA=RED+BLUE,YELLOW=RED+GREEN,WHITE=RED+GREEN+BLUE};
static const uint8_t colorArray[7]={WHITE,CYAN,BLUE,MAGENTA,RED,YELLOW,GREEN};
/*
#define WHITE   0xFFFFFF
#define BLACK   0x000000
#define RED     0xFF0000
#define GREEN   0x00FF00
#define BLUE    0x0000FF
#define CYAN    0x00FFFF
#define MAGENTA 0xFF00FF
#define YELLOW  0xFFFF00
*/
#define EE_COLOR 0x00
#define EE_NIGHTMODE 0x01
#define EE_ONHOUR 0x02
#define EE_OFFHOUR 0x03
#define EE_MODE 0x04

#define ILEST   18
#define HEURE   15
#define MIDI    13
#define MINUIT  3
#define MOINS   9
#define DIX     5
#define ETDEMIE 1
#define VINGT   11

#endif 
