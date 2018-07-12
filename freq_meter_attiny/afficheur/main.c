#define F_CPU 1000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#define verrou PINB3 //11
#define horloge PINB4 //12
#define data PINB1 //10

                        // |  8 digit bits, 0:On  | |  unused  | | BCD out |
volatile char state[16] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
volatile char digits[8] = {5, 4, 3, 1, 6, 2, 7, 0};
volatile char nums[8] = {0};
volatile n=0;
void shift(void);
void displaySingle(unsigned char digit, unsigned int nb);
void displayNumber(unsigned int number);
volatile i=0;
int main(void){

	DDRB |=1<<verrou | 1<<horloge | 1<<data;
	shift();
	TCCR0A|=0<<WGM02|0<<WGM01|0<<WGM00;//multiplex timer
	TIMSK|=1<<TOIE0 | 1<<TOIE1; // interrupts for both
	
	sei();
	
	TCCR1|=1<<CS13|1<<CS12|0<<CS10; // counter timer
	TCCR0B|=0<<CS02|1<<CS01|0<<CS00;// /8:010 /64:011 /1024:101
	
	while(1){
	}
	return 0;
}

ISR(TIMER0_OVF_vect){
	displaySingle(n,nums[n]);
	n=(n+1)%6;
}
ISR(TIMER1_OVF_vect){
displayNumber(i++);
	
}
void shift(void) {
  PORTB &=~(1<<verrou);
  int i;
  for (i = 0; i < 16; i++) {
	PORTB &=~(1<<horloge);
	if( state[i]){
	PORTB |=(1<<data);
	}else{
	PORTB &=~(1<<data);
	}
	PORTB |=(1<<horloge);
  }
	PORTB |=(1<<verrou);
}
void displayNumber(unsigned int number) {
	int i;
	for (i = 0; i<8; i++) {
		nums[i]=number%10;
		number/=10;
	}
}
void displaySingle(unsigned char d, unsigned int nb) {
	int i;
	for (i = 0; i < 7; i++) {//set to default 
		state[i] = 1;
	}
	for (i = 8; i < 16; i++) {//default
		state[i] = 0;
	}
	state[digits[d]] = 0;

	if (nb >= 8) {//convert decimal to binary in last 4bits
		state[12] = 1;
		nb -= 8;
	}
	if (nb >= 4) {
		state[13] = 1;
		nb -= 4;
	}
	if (nb >= 2) {
		state[14] = 1;
		nb -= 2;
	}
	if (nb >= 1) {
		state[15] = 1;
		nb -= 1;
	}
	shift();
}
