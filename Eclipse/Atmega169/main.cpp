#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
LCD myLCD;

uint16_t integerRoot(uint32_t* nb){
	int32_t i=15;
	uint32_t res=(1<<i);
	while(i>=0){
		if((res*res)>=(*nb))res-=(1<<i);
		res+=(1<<--i);
	}
	return res;
}

bool isPrime(uint32_t *i){
	uint16_t max=integerRoot(i);
	for(uint32_t j=3;j<max;j+=2){
		if((*i)%j==0)return false;
	}
	return true;
}
bool isPrimeDeprecated(uint32_t *i){
	uint16_t max=(*i)/2;
	for(uint32_t j=3;j<max;j+=2){
		if((*i)%j==0)return false;
	}
	return true;
}
int main(void){
	//uint32_t i=999979;
	  uint32_t i=999003;
	while(1){/*
		if(prime(i)){
			myLCD.setNb(i);
		}
		i+=2;
		myLCD.blink(1);
		if(i>999983)i=3;*/
		i+=2;
		if(isPrime(&i))myLCD.setNb(i);
		if(i>999983)i=3;
	}
	return 0;
}

