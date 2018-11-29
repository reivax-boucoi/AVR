#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "defines.h"
#include "Led.h"
#include "RTC.h"

void sendRawData(uint32_t data){
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
uint8_t ledr=0;
uint8_t ledb=0;
void sendData(uint32_t data){
    if(ledr)data|=LEDR;
    if(ledb)data|=LEDB;
    sendRawData(data);
}
volatile uint8_t state=0;
Led leds[NBLEDS];

int main(void){    
    DDRA |= CLK|DATA|STROBE|LED;
    PORTA &= ~(CLK|DATA|STROBE|LED);
    ledInit(leds);
    
    TIMSK1|=(1<<TOIE1);
    TIMSK0|=(1<<TOIE0);
    sei();
    
    sendData(0b11111100001111111111 | LEDB);
    _delay_ms(250);
    sendData(0b11000000001111111111);
    _delay_ms(250);
    sendData(0b00001100001111111111 | LEDR);
    _delay_ms(250);
    sendData(0b00110000001111111111);
    _delay_ms(250);
    sendData(0b00000100000000000001 | LEDB);
    #ifdef DELLONG
		TCCR1B|=(1<<CS12)|(1<<CS10);
	#else
		TCCR1B|=(1<<CS12);//|(1<<CS10);
	#endif
	
	TCCR0B|=(1<<CS01)|(1<<CS00);
    
    currentColor=tcolorV(WHITE);
    setCurrentTime(15,45,7,11);
	RTC_readTime(&currentTime);
    currentTime.temp=2;
    //setLeds(currentTime,leds,currentColor);
   setLedsNb(currentTime.temp,leds,currentColor);
    while(1){
       
    }
    return(0);
    
}
ISR( TIM1_OVF_vect ){
   RTC_readTime(&currentTime);
     uint8_t a=RTC_readTemp();
    if(currentTime.min%2){
        setLedsNb(currentTime.temp,leds,tcolorV(YELLOW));
        ledb=1;
        ledr=0;
    }else{
        ledr=1;
        ledb=0;
        setLeds(currentTime,leds,tcolorV(MAGENTA));
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
