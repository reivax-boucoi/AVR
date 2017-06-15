
//#define F_CPU 16000000
#define F_CPU 1000000

#include <avr/io.h>


int main(void){

TCCR0B |= (1<<CS02)|(0<<CS01)|(1<<CS00); 	// timer/counter control register 0, second bit | sets the clock prescaler to /1024

DDRB = 0b00000010;

PORTB &=~(1<<PINB1); // LED on PINB1

  while(1){

	if(TCNT0 > timer){ // counter register which holds the 8bit counter value
	TCNT0=0;
	PORTB ^= (1<<PINB1);	
	}
	
  }
  return 0;
}
