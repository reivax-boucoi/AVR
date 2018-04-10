#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define MINUS (1<<PC4)
#define DP (1<<PC3)
#define C (1<<PC2)
#define COMMA (1<<PC1)
#define E (1<<PC0)
// D0-D7 -> PD7-PD0 pull to GND to acitvate
// D8-D9 -> PB7-PB6
//
#define D (1<<PB5)
#define G (1<<PB4)
#define F (1<<PB3)
#define A (1<<PB2)
#define B (1<<PB1)
//					0-9, A,B,C,D,E,F
const uint8_t mapC[16]={E|C,C,E,C,C,C,E|C,C,E|C,C,
		C|E,C|E,E,E|C,E,E};
const uint8_t mapB[16]={A|B|D|F,B,A|B|G|D,A|B|D|G,B|G|F,A|F|G|D,A|F|D|G,A|B,A|B|F|G|D,A|B|F|G|D,
		A|B|F|G,A|B|F|G|D,A|D|F,A|B|D|F,A|F|G|D,A|F|G};
void setSeg(uint8_t n,int8_t i){
	for(uint8_t j=0;j<(7-i);j++){
		n/=10;
	}
	n%=10;
	PORTC=mapC[n];
	PORTB=mapB[n];
}
volatile int8_t i=7;
volatile uint8_t nb=0;
int main(void){
	DDRD|=0xFF; // digits port
	DDRB|=0xFE; // segment port + 2 digits + input on PB0
	DDRC|=0xFF; // segment port
	PORTD=0xFF;
	PORTC=0xFF;
	PORTB=0xFE; // input on PB0 OE
	DDRA=0x00;
	PORTA=0xFF;
	TCCR0|=(1<<WGM01);
	OCR0=5;
	TIMSK|=(1<<OCIE0);
	TCCR0|=(1<<CS02)|(1<<CS00);
	sei();
	while(1){
		if(bit_is_clear(PINB,PB0))nb=PINA;
	}
	return 0;
}
ISR(TIMER0_COMP_vect){
	if(i<5)i=7;
	setSeg(nb,i);
	PORTD=~(1<<i);
	i--;
}
