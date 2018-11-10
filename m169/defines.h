#ifndef DEFINES_H
#define DEFINES_H
#include <avr/io.h>

//blanking -> LOW pin 13

#define CLK (1<<PA1) //pin 15, store on rising edge
#define DATA (1<<PA0) //pin 27
#define STROBE (1<<PA2) //pin 16 latch on rising edge

#define NBLEDS 19

#define G1 0b10000000000000000000
#define G2 0b01000000000000000000
#define B2 0b00100000000000000000
#define B1 0b00010000000000000000
#define R1 0b00001000000000000000
#define R2 0b00000100000000000000

#define ILEST
#define HEURE
#define MIDI
#define MINUIT
#define MOINS
#define DIX
#define ETDEMIE
#define VINGT

#endif 
