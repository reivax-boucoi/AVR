
#ifndef DISPLAY_H
#define DISPLAY_H



#include <avr/io.h>
#include <avr/interrupt.h>

#define verrou PINB3 //11
#define horloge PINB4 //12
#define data PINB1 //10

class Display{
	public:
		Display();/*
		void shift();
		char n=0;
		char nums=[8]={0};
		*/
	private:
		/*displayNumber(unsigned int number);
		displaySingle(unsigned char d, unsigned int nb);
							 // |  8 digit bits, 0:On  | |  unused  | | BCD out |
		volatile char state[16] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
		volatile char digits[8] = {5, 4, 3, 1, 6, 2, 7, 0};*/
		
};

#endif