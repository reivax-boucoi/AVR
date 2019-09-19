
#define F_CPU 32768

#include <avr/io.h>
#include <util/delay.h>


int main(void){
  DDRB |= 1 << PINB0 ;
  PORTB |= 1<<PINB0 ;
	
  while(1){
	_delay_us(50000);
    PORTB^=(1<<PINB0);
  }
  return 0;
}
