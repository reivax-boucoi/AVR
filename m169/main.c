#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "defines.h"
#include "Led.h"
#include "RTC.h"

uint8_t ledMap[11]={0,0,0,0,0,0,0,0,0,0,0};

void sendData(uint32_t data){
    PORTA &=~(STROBE|CLK);
    for(uint8_t i=0;i<20;i++){
        if((data>>i) & 0x00000001){
            PORTA|=DATA;
        }else{
            PORTA&=~DATA;
        }
        PORTA|=CLK;
        PORTA&=~CLK;
    }
    PORTA|=STROBE;
    PORTA&=~STROBE;
}

volatile uint8_t state=0;
Led leds[NBLEDS];

int main(void){
    DDRB|=1<<PORTB0;
    PORTB &= ~(1<<PB0);
    
    DDRA |= CLK|DATA|STROBE;
    PORTA &= ~(CLK|DATA|STROBE);
    ledInit(leds);
    
    TIMSK1|=(1<<TOIE1);
    TIMSK2|=(1<<TOIE2);
    sei();
    
    sendData(0b11111100001111111111);
    _delay_ms(1000);
    
    TCCR1B|=(1<<CS12);//|(1<<CS10);
    TCCR2A|=(1<<CS22)|(1<<CS21);
    ledOn(&leds[0]);
    ledOn(&leds[2]);
    ledOn(&leds[4]);
    
    while(1){
    }
    return(0);
    
}
ISR( TIMER1_OVF_vect ){
    
    PORTB^=(1<<PB0);
}
ISR( TIMER2_OVF_vect ){
    switch(state){
        case 0:
            sendData(R2 | getDataByColor(1,0,0,0,leds));
            break;
        case 1:
            sendData(R1 | getDataByColor(1,0,0,1,leds));
            break;
        case 2:
            sendData(G2 | getDataByColor(0,1,0,0,leds));
            break;
        case 3:
            sendData(G1 | getDataByColor(0,1,0,1,leds));
            break;
        case 4:
            sendData(B2 | getDataByColor(0,0,1,0,leds));
            break;
        case 5:
            sendData(B1 | getDataByColor(0,0,1,1,leds));
            break;
    }
    state++;
    if(state>5)state=0;
}
