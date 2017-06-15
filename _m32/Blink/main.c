// Blinks an LED on Pin 8 (bottom left)

#define F_CPU 20000000

#include <avr/io.h>
#include <util/delay.h>


int main(void){
  DDRB |= 0b00000010;
  while(1){
	PORTB ^= 1<<PIN1;
	_delay_ms(500);
	
  }
  return 0;
}