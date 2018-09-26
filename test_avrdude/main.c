#define F_CPU 1000000UL
#include <avr/io.h>

int main(void){
    DDRB|=1<<PORTB0;
    PORTB |= 1<<PINB0;

    while(1){
        PORTB ^=1<<PINB0;
    }
    return(0);

}
