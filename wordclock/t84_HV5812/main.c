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
struct Menu* Mcurrent=0;
uint8_t Mindex=0;
Led leds[NBLEDS];

int main(void){    
    ledInit(leds);
    DDRA&=~(BTNINTERNAL|BTNOK);
    DDRB&=~BTNSELECT;
    PORTA|=(BTNINTERNAL|BTNOK);
    PORTB|=BTNSELECT;
    TIMSK1|=(1<<TOIE1);//|(1<<OCIE1B);
   // OCR1B=32768;
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
    
    setCurrentTime(0,10,7,12);
    // //RTC_setTime(currentTime,6,18);
    RTC_readTime(&currentTime);
    EEPROM_write(0,0xAA);
    EEPROM_write(1,0xBC);
    while(1){
    }
    return(0);
    
}
ISR( PCINT1_vect){
    if(PINB&BTNSELECT){
        Mindex=findNextEntry(Mcurrent,Mindex);
        TCNT1=65534;
    }
}
ISR( PCINT0_vect){
    if(PINA&BTNOK){
        if(!Mcurrent){
            Mcurrent=&M0main;//if not in menu mode
        }else{//enter menu and execute
            if(Mcurrent->fptr!=0 && Mcurrent->sub[Mindex].submenu==0)Mcurrent->fptr(Mindex);//check if function is associated and selected value is not a navigation move
            Mcurrent=getSubMenu(Mcurrent,Mindex);//get new menu
            Mindex=0;
            for(uint8_t i=0;i<NBLEDS;i++){
                ledOff(&leds[i]);
            }
        }
        TCNT1=65534;
    }
    if(PINA&BTNINTERNAL){
    }
}/*
ISR( TIM1_COMPB_vect){
}*/
ISR( TIM1_OVF_vect ){
    if(!Mcurrent){
        uint8_t m=getMode();
        RTC_readTime(&currentTime);
        if(isInAllowedTime(currentTime.hour)){
            if(m<2){
                setLeds(currentTime,leds);
            }else if(m<4){
                setLedsNb(RTC_readTemp(),leds);
            }
        }
        ledr=!ledr;
    }else{
        for(uint8_t i=0;i<NBLEDS;i++){
            if(Mcurrent->sub[Mindex].led==i){
                ledOn(&leds[i],WHITE);
            }else{
                ledOff(&leds[i]);
            }
        }
    }
}
ISR( TIM0_OVF_vect ){
    switch(state){
        case 0:
            sendData(R2 | getDataByColor(RED,0,leds));
            break;
        case 1:
            sendData(R1 | getDataByColor(RED,1,leds));
            break;
        case 2:
            sendData(G2 | getDataByColor(GREEN,0,leds));
            break;
        case 3:
            sendData(G1 | getDataByColor(GREEN,1,leds));
            break;
        case 4:
            sendData(B2 | getDataByColor(BLUE,0,leds));
            break;
        case 5:
            sendData(B1 | getDataByColor(BLUE,1,leds));
            break;
    }
    state++;
    if(state>5)state=0;
}
