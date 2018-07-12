#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
LCD myLCD;

uint16_t integerRoot(uint32_t x){
    uint16_t res=0;
    uint16_t add= 0x8000;
    for(int i=0;i<16;i++){
        uint16_t temp=res | add;
        uint32_t g2=(temp-1)*(temp-1);
        if (x>g2){
            res=temp;
        }
        add>>=1;
    }
    return res;
}

bool isPrime(uint32_t i){
	uint16_t max=integerRoot(i);
	for(uint32_t j=3;j<max;j+=2){
		if(i%j==0)return false;
	}
	return true;
}

int main(void){
	uint32_t i=3;
	uint32_t r=0;
	myLCD.setClk(true);
		while(1){
			_delay_ms(600);
			r=integerRoot(i)+2;
			myLCD.setDigit(5,i%10);
			myLCD.setDigit(4,(i/10)%10);
			myLCD.setDigit(3,(i/100)%10);
			myLCD.setDigit(2,(i/1000)%10);
			i++;
			myLCD.setDigit(1,r%10);
			myLCD.setDigit(0,(r/10)%10);

	}

	return 0;
}

