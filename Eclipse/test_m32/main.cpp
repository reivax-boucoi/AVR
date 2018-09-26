// Blinks an LED on Pin 8 (bottom left)

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
//#include "LCD.h"
/*//Maps which segments should be on for digit to be displayed
											//0,1,2,3,4,5,6,7,8,9,
uint8_t dispmap[20]={0x7E,0x30,0x6D,0x79,0x33,0x5B,0x5F,0x70,0x7F,0x7B,
										/A,b,C,d,E,F,H,L,P,U
										0x77,0x1F,0x4E,0x3D,0x4F,0x47,0x37,0xE,0x67,0x3E};
*/
class LCD{
public :
	LCD();
	virtual ~LCD();
private :

};

int main(void){

  DDRB |= (1 << PINB0) | (1<<PINB1);
  PORTB |= (1<<PINB0) | (0<PINB1);

  while(1){
		PORTB ^= 1<<PINB0 ;
		PORTB ^= 1<<PINB1 ;
		_delay_ms(2000);

  }
  return 0;
}
