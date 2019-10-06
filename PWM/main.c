
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

#define del 1000
#define TOFFSET 20
int val=0;
float t=0;
int state=0;
float fcorr(float x){
	return 0.0034*x*x*x-0.3274*x*x+14.919*x+0.657;
}
int main(void){
  DDRB |= 1 << PINB0 ;
  PORTB |= 1<<PINB0 ;
  //N=8 -> 	4kHz pwm
  TCCR0A|=(1<<COM0A1)|(1<<WGM01)|(1<<WGM00);
  TCCR0B|=(1<<CS00);
  OCR0A=0;
  
  
 //ADMUX =(1 << REFS1)|(1 << MUX0)|(1 << MUX1)|(1 << MUX2)|(1 << MUX3);//Int temp, ref=1.1V
  ADMUX =(1<<ADLAR)|(1 << MUX0);//Ext pot PB2

  ADCSRA =(1 << ADEN) |(1 << ADPS2) |(1 << ADPS1) |(1 << ADPS0);
	
  while(1){
	 OCR0A=fcorr(t);
	 t+=2;
	 if(t>32)t=0;
	_delay_ms(2000);
  }
  return 0;
}