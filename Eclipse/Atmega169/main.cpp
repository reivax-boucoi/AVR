#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
LCD myLCD;

int main(void){
	DDRB |= (1 << PB0);
	PORTB |= (1<<PB0);
	myLCD.setDigit(0,1);
	while(1){
		myLCD.blink(1);
		myLCD.setBattery(LCD::NONE);
		_delay_ms(128);
		/*myLCD.setBattery(LCD::EMPTY);
		_delay_ms(128);
		myLCD.setBattery(LCD::FIRST);
		_delay_ms(128);
		myLCD.setBattery(LCD::SECOND);
		_delay_ms(128);
		myLCD.setBattery(LCD::THIRD);
		_delay_ms(128);
		myLCD.setBattery(LCD::ONEBAR);
		_delay_ms(128);
		myLCD.setBattery(LCD::TWOBAR);
		_delay_ms(128);
		myLCD.setBattery(LCD::FULL);
		_delay_ms(128);
		myLCD.blink(2);
		for(uint8_t d=0;d<6;d++){
			for(uint8_t i=0;i<10;i++){
				myLCD.setDigit(d,i);
				_delay_ms(500);
			}
		}*/
	}
	return 0;
}
