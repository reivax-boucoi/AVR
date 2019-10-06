
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

#define del 1000
#define TOFFSET 20
int val=0;
float t=0;
int state=0;
float AOhmScale(float x){//range 0->10
	return 0.12*x*x*x-3.4183*x*x+47.51*x+1.4929;
}
float BOhmScale(float x){//range 0->31
	return 0.004*x*x*x-0.3545*x*x+15.262*x+0.3694;
}
float sat(float x){
	if(x<0.0)return 0.0;
	if(x>255.0)return 255.0;
	return x;
}
int main(void){
  DDRB |= (1 << PINB0)|(1<<PINB1)|(1<<PINB2) ;
  TCCR0A|=(1<<COM0B1)|(1<<WGM01)|(1<<WGM00);
  TCCR0B|=(1<<CS00);
  OCR0B=0;
  
  ADMUX =(1<<ADLAR)|(1 << MUX0);//Ext pot PB2

  ADCSRA =(1 << ADEN) |(1 << ADPS2) |(1 << ADPS1) ;
	
  while(1){
	/*  ADCSRA |= (1 << ADSC);         // start ADC measurement
    while (ADCSRA & (1 << ADSC) );
	if(ADCH>130 &&state){
		t+=10;
		OCR0A=t;
		if(t>255)t=0;
		state=1-state;
	}else if(ADCH<120 && !state){
		state=1-state;
	}
	*/
	OCR0B=AOhmScale(t/10.0);
	 t+=5;
	 if(t>100)t=0;
	_delay_ms(1500);
  }
  return 0;
}