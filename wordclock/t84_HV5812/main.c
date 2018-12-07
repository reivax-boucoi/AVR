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
    DDRA&=~(BTNSELECT|BTNOK);
    PORTA|=(BTNSELECT|BTNOK);
    TIMSK1|=(1<<TOIE1);
    TIMSK0|=(1<<TOIE0);
    GIMSK|=(1<<PCIE0);
    PCMSK0|=(1<<PCINT5);
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
    //RTC_readTime(&currentTime);
    setLeds(currentTime,leds,currentColor);
    //setLedsNb(currentTime.temp,leds,currentColor);
    while(1){
       /* if(PINA&BTNSELECT){//PCINT5
            ledb=1;
        }else{
            ledb=0;
        }*/
    }
    return(0);
    
}
ISR( PCINT0_vect){
    if(PINA&BTNSELECT){
        ledb=1-ledb;
    }
}
ISR( TIM1_OVF_vect ){
    ledr=!ledr;
    if(ledr){
        //RTC_readTime(&currentTime);
        setLeds(currentTime,leds,tcolorV(YELLOW));
    }else{
        setLedsNb(10/*RTC_readTemp()*/,leds,tcolorV(CYAN));
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
