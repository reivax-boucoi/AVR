// Blinks an LED on Pin 8 (bottom left)

#define F_CPU 1000000

#define BLANK 10
#define NB_DIGITS 4

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
					//0, 1  , 2  , 3  , 4  , 5  , 6  , 7  ,8, 9 ,BLANK,A, C  , E  , F  , H  , L  , P  , U  
uint8_t digits[19] = {1,0x4F,0x12,0x06,0x4C,0x24,0x20,0x0F,0,0x04,0x7F,8,0x31,0x30,0x38,0x48,0x71,0x18,0x41};
volatile uint8_t current[NB_DIGITS]={BLANK,BLANK,BLANK,BLANK};
volatile uint8_t cnt=0;
volatile uint16_t i=0;
volatile uint16_t j=0;
void displayNumber(uint32_t nb);

int main(void){
  DDRA |= 0x0F;
  DDRB |= 0x7F;
  PORTB|= 0x7F;
  PORTA|= 0x0F;
  
  TCCR0 |= (1<<CS01);
  OCR2 = 255;
  TCCR2 |= (1<<WGM21) | (1<<CS22) | (1<<CS21) | (1<<CS20);
  TIMSK |= (1<<TOIE0) | (1<<OCIE2);
  sei();
  displayNumber(i);
  while(1){
	  
  }
  return 0;
}

ISR(TIMER2_COMP_vect){
	j++;
	if(j>=1){
		displayNumber(i++);
		j=0;
	}
}

ISR(TIMER0_OVF_vect){

	PORTB=digits[current[cnt]];
	PORTA=1<<cnt;
	cnt++;
	if(cnt>3)cnt=0;
}

void displayNumber(uint32_t nb){
	uint8_t state = 0;
	uint8_t d = 0;
	for(;d<NB_DIGITS-1;d++){
		uint32_t ten=pow(10,NB_DIGITS-1-d);
		if(nb>=ten){
			current[d]=nb/ten;
			nb-=current[d]*ten;
			state=1;
		}else if(state == 0){
			current[d]=BLANK;
		}else{
			current[d]=0;
		}
	}
	current[NB_DIGITS-1]=nb;
}