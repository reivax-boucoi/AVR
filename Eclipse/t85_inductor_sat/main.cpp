<<<<<<< HEAD
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
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
	DDRB&=~(1<<PB4);//adc input pot
	DDRB&=~(1<<PB0);//input button
	PORTB|=(1<<PB0);//input button
	DDRB|=(1<<PB3);//output pulse
	ADMUX|=(1<<REFS0) |(1<<ADLAR)|(1<<MUX1);//left adjusted input single ended on PB4
	ADCSRA|=(1<<ADEN)|(1<<ADATE)|(1<<ADPS2)|(1<<ADPS0)|(1<<ADPS0)|(1<<ADIE);
	DIDR0|=(1<<ADC2D);
	OCR0A=4;
	TCCR0A|=(1<<COM0A0)|(1<<COM0A1);
	TIMSK |=(1<< OCIE0A)|(1<<TOIE0);
	sei();
	ADCSRA|=(1<<ADSC);
	TCCR0B |=(1<<CS00);

	while(1){
		toggled();
	}
	return 0;
}
ISR(TIMER0_OVF_vect){
	//if(state){
	//PORTB|=(1<<PB3);
	//}
}
ISR(TIMER0_COMPA_vect){
	//PORTB&=~(1<<PB3);
	if(OCR0A<127)PORTB^=(1<<PB3);
}
ISR(ADC_vect){
	OCR0A=ADCH;
}
=======
#include <avr/io.h>
#include <util/delay.h>


int main(void){
	DDRB&=~(1<<PB4);//adc input pot
	DDRB&=~(1<<PB0);
	PORTB|=(1<<PB0);//output pulse
	DDRB|=(1<<PB3);//button toggle
	ADMUX|=(1<<ADLAR)|(1<<MUX1);//left adjusted input single ended on PB4
	bool state=false;

	while(1){
		if(bit_is_clear(PINB,PB0)){
			state=!state;
			_delay_ms(1000);
		}
		if(state){
			PORTB^=(1<<PB3);
			_delay_ms(500);
		}
	}
	return 0;
}

>>>>>>> dc7401071dfdc2b612810fc95500e92b3facce31
