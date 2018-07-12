// Blinks an LED on Pin 8 (bottom left)

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>


int main(void){
  DDRB |= 1 << PINB0 | 1<<PINB1;
  PORTB |= 1<<PINB0 | 0<PINB1;
	
  while(1){
		PORTB ^= 1<<PINB0 ;
		PORTB ^= 1<<PINB1 ;
		_delay_ms(500);
	
  }
  return 0;
}