#include <avr/io.h>
#include <avr/interrupt.h>

#define  EN PB2
#define  DIR PB1
#define  STEP PB0
#define MAXSTEP 158

#define

volatile uint8_t pos=0;
volatile uint8_t period=0;
void toggle(void){
	PORTB^=(1<<STEP);
	pos++;
	if(pos>=MAXSTEP){
		pos=0;
		PORTB^=(1<<DIR);
	}
}
int main(void){
	DDRB|=(1<<DIR)|(1<<STEP)|(1<<EN);
	for(uint8_t i=0;i<10;i++){
		toggle();
	}
	pos=0;
	TCCR0A|=(1<<WGM01);
	OCR0A=255;
	TIMSK|=(1<< OCIE0A);
	sei();
	TCCR0B |=(1<<CS00);
	while(1){
	}
	return 1;
}

ISR(TIM0_COMPA_vect){

}
