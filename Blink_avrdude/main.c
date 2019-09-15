// Blinks an LED on Pin 8 (bottom left)

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>


int main(void){
  DDRD |= 1 << PIND7 ;
  PORTD |= 1<<PIND7 ;
	
  while(1){
		PORTD ^= 1<<PIND7 ;
		_delay_ms(500);
	
  }
  return 0;
}