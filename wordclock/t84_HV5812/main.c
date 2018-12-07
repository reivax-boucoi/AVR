#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "defines.h"
#include "Led.h"
#include "RTC.h"
#include "Menu.h"

uint8_t ledr=0;
uint8_t ledb=0;
void sendData(uint32_t data){
    if(ledr)data|=LEDR;
    if(ledb)data|=LEDB;
    sendRawData(data);
}


volatile uint8_t state=0;
volatile struct Menu* Mcurrent=0;
volatile uint8_t Mindex=0;
Led leds[NBLEDS];

int main(void){    
    ledInit(leds);
    DDRA&=~(BTNINTERNAL|BTNOK);
    DDRB&=~BTNSELECT;
    PORTA|=(BTNINTERNAL|BTNOK);
    PORTB|=BTNSELECT;
    TIMSK1|=(1<<TOIE1);
    TIMSK0|=(1<<TOIE0);
    GIMSK|=(1<<PCIE1)|(1<<PCIE0);
    PCMSK0|=(1<<PCINT7)|(1<<PCINT5);
    PCMSK1|=(1<<PCINT10);
    sei();
    
    sendData(0b11111100001111111111);//white
    _delay_ms(500);
    sendData(0b11000000001111111111);//green
    _delay_ms(500);
    sendData(0b00001100001111111111);//red
    _delay_ms(500);
    sendData(0b00110000001111111111);//blue
    _delay_ms(500);
    sendData(0b11110000001111111111);//cyan
    _delay_ms(500);
    sendData(0b00111100001111111111);//magenta
    _delay_ms(500);
    sendData(0b11001100001111111111);//yellow
    _delay_ms(500);
    
    #ifdef DELLONG
    TCCR1B|=(1<<CS12)|(1<<CS10);
    #else
    TCCR1B|=(1<<CS12);//|(1<<CS10);
    #endif
    
    TCCR0B|=(1<<CS01)|(1<<CS00);
    
    currentColor=tcolorV(WHITE);
    setCurrentTime(0,10,7,12);
   // //RTC_setTime(currentTime,6,18);
    RTC_readTime(&currentTime);
    setLeds(currentTime,leds,currentColor);
    //setLedsNb(currentTime.temp,leds,currentColor);
    while(1){
    }
    return(0);
    
}
ISR( PCINT1_vect){
    if(PINB&BTNSELECT){
        ledb=1-ledb;
        ledr=0;
    }
}
ISR( PCINT0_vect){
    if(PINA&BTNOK){
        ledr++;
    }
    if(PINA&BTNINTERNAL){
        ledr--;
    }
}
ISR( TIM1_OVF_vect ){
    if(ledr){
        RTC_readTime(&currentTime);
        setLeds(currentTime,leds,tcolorV(YELLOW));
    }else{
        setLedsNb(RTC_readTemp(),leds,tcolorV(CYAN));
    }
    
}
ISR( TIM0_OVF_vect ){
    switch(state){
        case 0:
            sendData(R2 | getDataByColor(tcolor(1,0,0),0,leds));
            break;
        case 1:
            sendData(R1 | getDataByColor(tcolor(1,0,0),1,leds));
            break;
        case 2:
            sendData(G2 | getDataByColor(tcolor(0,1,0),0,leds));
            break;
        case 3:
            sendData(G1 | getDataByColor(tcolor(0,1,0),1,leds));
            break;
        case 4:
            sendData(B2 | getDataByColor(tcolor(0,0,1),0,leds));
            break;
        case 5:
            sendData(B1 | getDataByColor(tcolor(0,0,1),1,leds));
            break;
    }
    state++;
    if(state>5)state=0;
}
