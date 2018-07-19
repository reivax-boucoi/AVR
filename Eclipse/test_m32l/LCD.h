/*
 * LCD.h
 *
 *  Created on: 18 févr. 2018
 *      Author: User
 */
#include <avr/io.h>
#ifndef LCD_H_
#define LCD_H_

/*//Maps which segments should be on for digit to be displayed
											//0,1,2,3,4,5,6,7,8,9,
uint8_t dispmap[20]={0x7E,0x30,0x6D,0x79,0x33,0x5B,0x5F,0x70,0x7F,0x7B,
										/A,b,C,d,E,F,H,L,P,U
										0x77, 0x1F,0x4E,0x3D,0x4F,0x47,0x37,0xE,0x67,0x3E};
*/

enum Symbols {A=0x77,b=0x1F,C=0x4E,d=0x3D,E=0x4F,F=0x47,H=0x37,L=0xE,P=0x67,U=0x3E};
enum Specials {EMPTY=0x20,ONEBAR=0x30,TWOBAR=0x38,FULL=0x3C,CLK=0x2,DOT=0x1};

class LCD {
public:


	LCD();
	~LCD();
	int8_t DisplayNB(float nb);
	int8_t DisplayNB(int32_t nb);
	Specials getSpec() const;
	void setSpec(Specials spec);
	const Symbols getDigits(uint8_t d) const;

private:
	Symbols digits[6];
	Specials spec;
};

#endif /* LCD_H_ */
