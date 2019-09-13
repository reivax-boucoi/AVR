#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
//#include "PM.h"
#define BLUE_LED (1<<PD5)
#define YELL_LED (1<<PD6)
#define BUTTON (1<<PD2)

#define BUTTON_STATE !( PIND & BUTTON)

void GPIO_init(void){
    DDRD|=YELL_LED|BLUE_LED;
    DDRD&=~BUTTON;
    PORTB|=BUTTON;
}
int main(void){
    
    GPIO_init();
    
    while(1){
        
        _delay_ms(1000);
        PORTD^=YELL_LED;
        if(BUTTON_STATE){
            PORTD|=BLUE_LED;
        }else{
            PORTD&=~BLUE_LED;
        }
        
    }
    return 0;
}
