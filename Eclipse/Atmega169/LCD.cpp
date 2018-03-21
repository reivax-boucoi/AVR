#include "LCD.h"

LCD::LCD() {
	LCDCRB|=(1<<LCDMUX1)|(1<<LCDMUX0);
	LCDFRR|=(1<<LCDPS2)|(1<<LCDCD2); // prescaler 512, division factor 5 & K=8 for duty 1/4
	LCDCCR|=(1<<LCDCC1)|(1<<LCDDC2)|(1<<LCDDC1);
	LCDCRA|=(1<<LCDEN);
}

LCD::~LCD() {
	LCDCRA|=(1<<LCDBL);
	_delay_ms(500);
	LCDCRA&=~(1<<LCDEN);
}

void LCD::setDigit(uint8_t d, uint8_t nb) {
//nothing on 0,1,2,3,4,7,8,9,12,13,14,
	//LCDDR5|=0xFF; digits 5,4,3,2 segs B and G
	//LCDDR6|=0xFF; //digits 1,0 segs B and G, + T2
	//LCDDR10|=0xFF; //digits 5,4,3,2 segs E and C
	//LCDDR11|=0xFF; //digits 1,0 segs E and C
	//LCDDR15|=0xFF; //digits 5,4,3,2 D and DP
	//LCDDR16|=0xFF; //digits 1,0 D and CLK and T3

	for(uint8_t i=0;i<8;i++){
		blink(1);
		LCDDR16=(1<<i);
		_delay_ms(1500);
	}
	/*blink(1);
	LCDDR0|=0xFF; //digits 1,0 D and CLK and T4
	_delay_ms(1500);
	blink(2);
	LCDDR1|=0xFF; //digits 1,0 D and CLK and T4
	_delay_ms(1500);
	blink(3);
	LCDDR2|=0xFF; //digits 1,0 D and CLK and T4
	_delay_ms(1500);
	blink(4);
	LCDDR3|=0xFF; //digits 1,0 D and CLK and T4
	_delay_ms(1500);
	blink(5);
	LCDDR5|=0xFF; //digits 1,0 D and CLK and T4
	_delay_ms(1500);
	blink(6);
	LCDDR6|=0xFF; //digits 1,0 D and CLK and T4
	_delay_ms(1500);
	blink(7);
	LCDDR7|=0xFF; //digits 1,0 D and CLK and T4
	_delay_ms(1500);
	blink(8);
	LCDDR8|=0xFF; //digits 1,0 D and CLK and T4
	_delay_ms(1500);
	blink(9);
	LCDDR10|=0xFF; //digits 1,0 D and CLK and T4
	_delay_ms(1500);
	blink(10);
	LCDDR11|=0xFF; //digits 1,0 D and CLK and T4
	_delay_ms(1500);
	blink(11);
	LCDDR12|=0xFF; //digits 1,0 D and CLK and T4
	_delay_ms(1500);
	blink(12);
	LCDDR13|=0xFF; //digits 1,0 D and CLK and T4
	_delay_ms(1500);
	blink(13);
	LCDDR15|=0xFF; //digits 1,0 D and CLK and T4
	_delay_ms(1500);
	blink(14);
	LCDDR16|=0xFF; //digits 1,0 D and CLK and T4
	_delay_ms(1500);
	blink(15);
	LCDDR17|=0xFF; //digits 1,0 D and CLK and T4
	_delay_ms(1500);
	blink(16);
	LCDDR18|=0xFF; //digits 1,0 D and CLK and T4
	_delay_ms(1500);*/
}

void LCD::blink(uint8_t t) {
	for(int i=0;i<t;i++){
		PORTB|=(1<<PB0);
		_delay_ms(15);
		PORTB&=~(1<<PB0);
		_delay_ms(300);
	}
}
