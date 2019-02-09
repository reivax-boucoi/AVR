#include <avr/io.h>
#include <avr/interrupt.h>
#define LENGTH 11
static const uint8_t array[LENGTH]={118,94,79,59,46,39,46,59,79,94,118};
volatile uint8_t i=0;
int main(void){
	DDRB|=(1<<PB0);
	DDRD|=(1<<PD6);

	TCCR0A|=(1<<COM0A0)|(1<<WGM01);
	TCCR1B|=(1<<WGM12);
	OCR0A=0;
	OCR1A=16384;
	TIMSK1|=(1<<OCIE1A);

	sei();
	TCCR0B|=(1<<CS02);//|(1<<CS00);//256
	TCCR1B|=(1<<CS12)|(1<<CS10);

	while(1){

	}
	return 1;
}
ISR(TIMER1_COMPA_vect){
	PORTB^=(1<<PB0);
	OCR0A=array[i];
	i++;
	if(i>LENGTH)i=0;
}
