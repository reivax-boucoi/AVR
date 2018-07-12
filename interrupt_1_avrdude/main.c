#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t cnt=0;
uint8_t state=0;

int main(void){

TCCR0A |= (1<<WGM01); //sets to CTC (clear on match with OCR0A)

OCR0A = 255; // value to compare and clear at 

TIMSK0 |=(1<<OCIE0); // enable set of interrupt on match

sei(); // I-bit in SREG

TCCR0B |=(1<<CS02)|(0<<CS01)|(1<<CS00); // set prescaler to /1024

DDRB |= 0b00000001; // LED output on PINB1

PORTB &=~(1<<PINB1); // LED on PINB1

	while(1){
		
	}
	return 0;
}

ISR(TIMER0_COMPA_vect){
	
	cnt++;
	if(cnt>10){
	cnt=0;
	PORTB ^= (1<<PINB0);
	}
 }