#include <avr/io.h>
#include <util/delay.h>


int main(void){
	DDRB&=~(1<<PB4);//adc input pot
	DDRB&=~(1<<PB0);
	PORTB|=(1<<PB0);//output pulse
	DDRB|=(1<<PB3);//button toggle
	ADMUX|=(1<<ADLAR)|(1<<MUX1);//left adjusted input single ended on PB4
	bool state=false;

	while(1){
		if(bit_is_clear(PINB,PB0)){
			state=!state;
			_delay_ms(1000);
		}
		if(state){
			PORTB^=(1<<PB3);
			_delay_ms(500);
		}
	}
	return 0;
}

