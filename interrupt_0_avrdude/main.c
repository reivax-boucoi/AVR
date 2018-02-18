#define F_CPU 1000000
#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t cnt=0;

int main(void){

TCCR0A = (1<<WGM01); //sets to CTC (clear on match with OCR0A)

OCR0A = 255; // value to compare TCNT0 to and clear at 

TIMSK0 =(1<<OCIE0A); // enable set of interrupt on match

sei(); // I-bit in SREG

TCCR0B =(1<<CS02)|(0<<CS01)|(1<<CS00); // set prescaler to /1024 | also starts timer -> end of setup

DDRB |=1<<PINB3; // LED output on PINB1

PORTB &=~(1<<PINB3); // LED on PINB1

	while(1){
		
	}
	return 0;
}

ISR(TIMER0_COMPA_vect){
	
	cnt++;
	if(cnt>3){
	cnt=0;
	PORTB ^= (1<<PINB3);
	}
 }