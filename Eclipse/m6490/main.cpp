#include <avr/io.h>
#include <util/delay.h>

//float LCDval=0.0;
int main(void){
	DDRF|=(1<<PF0);
	LCDCRB|=(1<<LCDMUX0)|(1<<LCDMUX1)|(1<<LCDPM3)|(1<<LCDPM2)|(1<<LCDPM1)|(1<<LCDPM0);
	LCDFRR|=(1<<LCDPS0)|(1<<LCDPS1)|(1<<LCDPS2)|(1<<LCDCD2);	//8MHz/(4096*8*5)=48.8Hz
	LCDCCR|=(1<<LCDCC3);
	LCDCRA|=(1<<LCDEN);
	LCDDR00=(1<<2);
	while(1){
		PORTF^=(1<<PF0);
		_delay_ms(500);
	}
	return 1;
}

