#include <avr/io.h>
#include <avr/interrupt.h>

int main(void){
	DDRC|=(1<<PC0);//OC0A
	DDRB|=(1<<PB0);
	PORTC|=(1<<PC0);
	TCCR0A|=(1<<COM0A0)|(1<<WGM01);
	OCR0A=127;
	ADMUX|=7;
	ADCSRA|=(1<<ADEN)|(1<<ADATE)|(1<<ADIE)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
	ADCSRB|=(1<<ADLAR)|(1<<ADTS1)|(1<<ADTS0);
	TIMSK|=(1<<OCIE0A);
	sei();
	TCCR0B|=(1<<CS01);
	while(1){
	}
	return 1;
}
ISR(ADC_READY_vect){
	OCR0A=ADCH;
	PORTB|=(1<<PB0);
}
