#include <avr/io.h>
#include <util/delay.h>
#include<avr/interrupt.h>

#define LED_HIGH (PORTB |= 1<<PINB0)
#define LED_LOW (PORTB &=~(1<<PINB0))
#define LED_TOGGLE (PORTB ^=(1<<PINB0))

void blink(int nb){
    for(int i=0;i<nb;i++){
        LED_HIGH;
        _delay_ms(15);
        LED_LOW;
        _delay_ms(85);
    }
}
const unsigned int multiplier=3600;
volatile unsigned int cnt=0;
volatile unsigned long setTime;
volatile unsigned long remainingTime;
volatile uint8_t showRemaining=0;
volatile unsigned int nextShowTime;
volatile long showValueLeft;

int main(void){
    DDRB |= (1 << PINB0)|(1 << PIND5) ; //LED && relay init
    LED_LOW;
    PORTD &=~(1<<PIND5);
    
    DDRD &=~(1<<PIND6);//Button
    PORTD |=(1<<PIND6);
    
    DDRB &=~(0x1E); //sw init
    PORTB |=(0x1E);
    
    TIMSK |=(1<<TOIE1)|(1<<OCIE1A);//timer init
	TCCR1A = 0x00;
    TCNT1 = 0x0000; 
    OCR1A = 625-1; //4MHz/8=500kHz -> /625=100Hz
    
    sei();//enable global interrupts
    
    while(1){
        if(!(PIND&(1<<PIND6))){
            setTime=(15-((PINB & 0x1E)>>1))*multiplier;
            remainingTime=setTime;
            TCCR1B |=(1<<CS11)|(1<<WGM12); // /1024, CTC on OCR1A
            PORTD |=(1<<PIND5);
        }
        
    }
    return 0;
}     


ISR (TIMER1_OVF_vect){
}

ISR(TIMER1_COMPA_vect){//occurs @100Hz
    cnt++;
    if(cnt>=300){//occurs every 3s
        remainingTime-=3;
        if(remainingTime==0){//stop counter and de-energize relay
            LED_HIGH;
            PORTD &=~(1<<PIND5);
            TCCR1B=0;
        }
        showRemaining=1;
        showValueLeft=remainingTime-multiplier;
        cnt=0;
    }
    
    if(showRemaining && (cnt&0x10)){
        LED_TOGGLE;
        showValueLeft-=multiplier/16;
        if(showValueLeft<=0){
            showRemaining=0;
            LED_LOW;
        }
    }
}
