#include <avr/io.h>
#include <util/delay.h>
void display(int number);
#define segA PIND0
#define segB PIND2
#define segC PIND1
#define segD PIND1
#define segE PIND3
#define segF PIND0
#define segG PIND2



int main(void){
  DDRD |= 0b00001111;
  DDRB |=0b000000011;
  while(1){
display(7);
_delay_ms(1000);
display(1);
_delay_ms(1000);
display(10);
_delay_ms(1000);
	
  }
  return 0;
}


void display(int number){
  switch (number)
  {
    case (0):
    // a b c d e f ;
      break;

    case (1):
	PORTB = 0b00000010;
    PORTD = 1<< segB | 1<< segC;// b c  ;
      break;

    case (2):
    //  a b g e d ;
      break;

    case (3):
    // a b c d g ;
      break;

    case (4):
    //  b c f g ;
      break;

    case (5):
    // a b g e d ;
      break;
	  
	case (6):
	 //a f g e d c ;
      break;
	  
    case (7):
	PORTB = 0b00000010;
    PORTD = 1<< segA | 1<< segB | 1<< segC; //a b c  ;
      break;

    case (8):
     //a b c d e f g ;
      break;
	  
	case (9):
	 //a b c d f g ;
      break;

    default:
	PORTB = 0b00000000;
     PORTD = 0x00 ;
      break;
  }


}