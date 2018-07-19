#include <avr/io.h>
#include "include/LCD.h"
//float LCDval=0.0;
int main(void){
	DDRF|=(1<<PF0);

	while(1){
		PORTF^=(1<<PF0);
		LCDDR00=0xFF;
		LCDDR01=0xFF;
		LCDDR02=0xFF;
		LCDDR03=0xFF;
		LCDDR04=0xFF;
		LCDDR05=0xFF;
		LCDDR06=0xFF;
		LCDDR07=0xFF;
		LCDDR08=0xFF;
		LCDDR09=0xFF;
		LCDDR10=0xFF;
		LCDDR11=0xFF;
		LCDDR12=0xFF;
		LCDDR13=0xFF;
		LCDDR14=0xFF;
		LCDDR15=0xFF;
		LCDDR16=0xFF;
		LCDDR17=0xFF;
		LCDDR18=0xFF;
		LCDDR19=0xFF;
		_delay_ms(500);
		LCDDR00=0x00;
		LCDDR01=0x00;
		LCDDR02=0x00;
		LCDDR03=0x00;
		LCDDR04=0x00;
		LCDDR05=0x00;
		LCDDR06=0x00;
		LCDDR07=0x00;
		LCDDR08=0x00;
		LCDDR09=0x00;
		LCDDR10=0x00;
		LCDDR11=0x00;
		LCDDR12=0x00;
		LCDDR13=0x00;
		LCDDR14=0x00;
		LCDDR15=0x00;
		LCDDR16=0x00;
		LCDDR17=0x00;
		LCDDR18=0x00;
		LCDDR19=0x00;
		_delay_ms(500);
	}
	return 1;
}

