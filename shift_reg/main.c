
#include <avr/io.h>
#include <util/delay.h>


#define SIn PIND0	// pin 14, serial input

#define RClk PIND1	// pin 12, storage reg clock

#define SRClk PIND2	// pin 11, shift reg clock

// SRCLR pin 10 to HIGH
// OE pin 13 to LOW

void writeReg();

int values[8]={0,1,0,1,0,1,0,1};

int main(void){
DDRD = 0b00000111;
PORTD |= 0b00000110;
	_delay_ms(5000);

	while(1){
	writeReg();
	_delay_ms(2500);
	}

  return 0;
}
void writeReg(){
	PORTD &= ~(1<< RClk); //0
	_delay_ms(1000);

	int i=0;
	  for(i=0;i<8;i++)
	  {
		PORTD &= ~(1<< SRClk); // 0
		_delay_ms(1000);
		if(values[i]){
			PORTD |= 1<<SIn; // 1
		}else{
			PORTD &=~(1<<SIn); // 0
		}
		_delay_ms(1000);
		PORTD |= 1<< SRClk;		//1
	}
	
	_delay_ms(1000);
	PORTD |= 1<< RClk; // 1
}