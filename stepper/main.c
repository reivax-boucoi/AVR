// Blinks an LED on Pin 8 (bottom left)

#define F_CPU 1000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DIR PINB1
#define STEP PINB0
#define ACC_MAX 1
#define ACC_MIN 255
#define VEL_MAX 4
#define VEL_MIN 255

void setAcc(uint8_t acc);
void setVel(uint8_t vel,uint8_t smooth);
uint8_t map(uint8_t val,uint8_t min1,uint8_t max1,uint8_t min2,uint8_t max2);
uint8_t getVel();
uint8_t getAcc();
void gotoSpeed(uint8_t speed);
void setDir(uint8_t dir);
uint8_t getDir();
void stop(uint8_t smooth);
void start();

volatile uint8_t tvel=0;;

int main(void){
	
	setAcc(10);
	setVel(50,0);
	start();
	_delay_ms(5000);
  while(1){
	gotoSpeed(0);
	_delay_ms(1000);
	gotoSpeed(100);
	_delay_ms(1000);
	
  }
	stop(1);
  return 0;
}

ISR(TIMER0_COMPA_vect){
	PORTB ^= (1<<STEP);
	TCNT0=0;
 }
 
 ISR(TIMER1_COMPA_vect){
	 //PORTB ^= (1<<DIR);
	 if(OCR0A>tvel){
		 OCR0A--;
	 }else if(OCR0A<tvel){
		 OCR0A++;
	 }
	 TCNT1=0;
	 
 }
 
 uint8_t map(uint8_t val,uint8_t min1,uint8_t max1,uint8_t min2,uint8_t max2){
	 return min2+((val-min1)*(max2-min2)/(max1-min1));
 }

void setAcc(uint8_t acc){
	OCR1A = map(acc,0,100,ACC_MIN,ACC_MAX);
}
void setVel(uint8_t vel,uint8_t smooth){
	uint8_t speed = map(vel,0,100,VEL_MIN,VEL_MAX);
	if(!smooth)OCR0A=speed;
	tvel=speed;
}
uint8_t getVel(){
	return map(OCR0A,VEL_MIN,VEL_MAX,0,100);
}
uint8_t getAcc(){
	return map(OCR1A,ACC_MIN,ACC_MAX,0,100);
}
void gotoSpeed(uint8_t speed){
	tvel=map(speed,0,100,VEL_MIN,VEL_MAX);
	while(OCR0A!=tvel);
}
void setDir(uint8_t dir){
	if(dir){
		PORTB |= (1<<DIR);
	} else {
		PORTB &= ~(1<<DIR);
	}
}
uint8_t getDir(){
	return PORTB & (1<<DIR);
}
void stop(uint8_t smooth){
	if(smooth){
		tvel=VEL_MIN;
		while(OCR0A<VEL_MIN);
	}
	cli();
}
void start(){
	DDRB |= 1 << DIR |1<<STEP;
	PORTB &= ~( 1<<DIR |1<<STEP);
	TCCR0A = (1<<WGM01);
	TCCR1 = (1<<CS13)|(1<<CS12)/*|(1<<CS11)*/|(1<<CS10);
	OCR0A = 255; // sets speed : 4:fastest, 255 : slowest
	OCR1A = 4; // sets acceleration : 1:fastest speed ramp, 255 slowest ramp
	TIMSK = (1<<OCIE0A)|(1<<OCIE1A);
	sei();
	TCCR0B =(1<<CS02)|(0<<CS01)|(0<<CS00); //256
}