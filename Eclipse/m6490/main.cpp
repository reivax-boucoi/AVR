#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "include/LCD.h"

int8_t i=1;
LCD myLCD;
int main(void){
	DDRB|=(1<<PB0);
	myLCD.clear();
	//myLCD.setPlus(true);
	LCDDR00=0xff;
	LCDDR01=0xff;
	LCDDR02=0xff;
	LCDDR03=0xff;
	LCDDR04=0xff;
	LCDDR05=0xff;
	LCDDR06=0xff;
	LCDDR07=0xff;
	LCDDR08=0xff;
	LCDDR09=0xff;
	LCDDR10=0xff;
	LCDDR11=0xff;
	LCDDR12=0xff;
	LCDDR13=0xff;
	LCDDR14=0xff;
	LCDDR15=0xff;
	LCDDR16=0xff;
	LCDDR17=0xff;
	LCDDR18=0xff;
	LCDDR19=0xff;
	int i=0;
	while(1){
		PORTB^=(1<<PB0);
		_delay_ms(350);
		LCDDR06=1<<i;
		_delay_ms(200);
	   LCDDR06|=1<<(i+1);
		_delay_ms(600);
		LCDDR06=0x00;
	}
	return 1;
}


