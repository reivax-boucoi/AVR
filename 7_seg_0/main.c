// Blinks an LED on Pin 8 (bottom left)

#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile char value=10;
void display(char nb);
volatile char state=0;
int cnt;
float f=5; // frequence (approx) chgt value
int modul;

int main(void){

	DDRB |= 0b00000011; // dizaines et unites enable pour multiplex
	DDRD |= 0b00001111; // bcd output port

	PORTB=0b00000010;
	PORTD=0b00000000;
	// timer pour persistance retinienne (10ms) + chgt value
	TCCR0A = (1<<WGM01); // CTC on update
	OCR0A = 10; // update rate, fixee pour persistance !
	TIMSK0 =(1<<OCIE0A); // interrupt on compare match
	
	sei();
	
	TCCR0B =(1<<CS02)|(0<<CS01)|(1<<CS00); // display update timer (/1024)
	modul = (int)(F_CPU/(1024*OCR0A*f)); // calcul div interrupt pour update selon f
	
	while(1){
	display(value);
	}
	return 0;
}



void display(char nb){
	
	if(nb>99) nb=99; // constrain
	if(nb<0) nb=0;
	
	char temp=nb/10; // dizaines
	
	if(state){ // display alternativement unites et dizaines
	PORTD=temp;
	PORTB |= 1<<PINB1;
	PORTB &= ~(1<<PINB0);
	}else{
	PORTD=nb-(10*temp);
	PORTB |= 1<<PINB0;
	PORTB &= ~(1<<PINB1);
	}
	state=1-state; // change pour prochain display
}

ISR(TIMER0_COMPA_vect){ // timer persistance 
	display(value); // persistance ! 
	cnt++;
	
	if(cnt%modul==0){ // avancement compte
	
	cnt=0;
	value++;
	if(value>99) value=0;
	
	}
	
}