#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "include/LCD.h"

int8_t i=1;
LCD myLCD;
int main(void){
	DDRB|=(1<<PB0);
	myLCD.clear();
	//myLCD.setBattery(Battery::FIRST);
	//myLCD.setBargraphMode(Bargraph::STACK);
	//myLCD.setBargraphLevel(11);
	//myLCD.setArrow(Arrow::DOWN,true);
	//myLCD.setArrow(Arrow::UP,true);
	//myLCD.setArrow(Arrow::RIGHT,true);
	//myLCD.setArrow(Arrow::LEFT,true);
	//myLCD.setMinus(true);
	//myLCD.setPlus(true);
	while(1){
		PORTB^=(1<<PB0);
		_delay_ms(100);
		myLCD.setPlus(true);
		_delay_ms(150);
		myLCD.setMinus(true);
		_delay_ms(600);
		myLCD.setPlus(false);
		_delay_ms(150);
		myLCD.setMinus(false);
	}
	return 1;
}


