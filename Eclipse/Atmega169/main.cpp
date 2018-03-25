#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
LCD myLCD;
bool prime(uint32_t i){
	for(uint32_t j=3;j<i/2;j+=2){
		if(i%j==0)return false;
	}
	return true;
}
int main(void){
	uint32_t i=999979;
	while(1){
		if(prime(i)){
		myLCD.setNb(i);
		}
		i+=2;
		myLCD.blink(1);
		if(i>999983)i=3;
	}
	return 0;
}
