#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define LED (1<<PB1)
#define OUT (1<<PB3)
#define BUTTON (1<<PB0)
#define POT (1<<PB4)
#define CONFLEVEL 200

uint8_t bconf=0;
uint8_t aconf=0;
bool bstate=false;
bool state=false;

void toggled(void){
	if(bit_is_clear(PINB,PB0)){
		if(bconf<255)bconf++;
		aconf=0;
		if(bconf>CONFLEVEL && !bstate){
			bstate=true;
			bconf=0;
			state=!state;
		}
	}else{
		aconf++;
		bconf=0;
		if(aconf> CONFLEVEL && bstate){
			bstate=0;
			aconf=0;
		}
	}
}

int main(void){
	DDRB&=~POT;//adc input pot
	DDRB&=~BUTTON;//input button
	PORTB|=BUTTON;//input button
	DDRB|=OUT|LED;//output pulse & LED
	//ADMUX|=(1<<REFS0) |(1<<ADLAR)|(1<<MUX1);//left adjusted input single ended on PB4
	//ADCSRA|=(1<<ADEN)|(1<<ADATE)|(1<<ADPS2)|(1<<ADPS0)|(1<<ADPS0)|(1<<ADIE);
	//DIDR0|=(1<<ADC2D);

	OCR0B=254;
	OCR0A=120;
	//TCCR0A|=(1<<WGM01);//(1<<COM0B0)|(1<<COM0B1);
	TIMSK |=(1<<TOIE0);
	sei();

	//ADCSRA|=(1<<ADSC);

	TCCR0B |=(1<<CS00)|(1<<CS02);

	while(1){
		toggled();
	}
	return 0;
}
ISR(TIMER0_OVF_vect){
	PORTB
}
ISR(ADC_vect){
	//OCR0A=ADCH;
}
