#include <avr/io.h>
#include <avr/interrupt.h>

int main(void){
	DDRB|=(1<<PB6)|(1<<PB7)|(1<<PB5);
	TCCR1A|=(1<<COM1A0)|(1<<COM1A1);
	TCCR1B|=(1<<WGM12);
	OCR1A=0x00FF;
	TIMSK1|=(1<<TOIE1);
	sei();
	TCCR1B|=(1<<CS10);
	while(1){
	}
	return 1;
}
