#include <avr/io.h>
#include <util/delay.h>
#include "include/cmds.h"
#include "include/periphs.h"
#include "include/LCD.h"

LCD myLCD;
float LCDval=0.0;

int main(void){
	periphs_init();
	_delay_ms(1000);
	sei();
	uart_transmitnl("\tHello World !");
	uart_prompt();
	while(1){

	}
	return 0;
}

ISR(USART0_RX_vect){
	uart_isr_rxc();
}

ISR(USART0_UDRE_vect){
	uart_isr_udre();
}

ISR(TIMER2_OVF_vect){
	PORTB^=(1<<PB0);
}

ISR(TIMER1_OVF_vect){
	LCDval+=.01;
	myLCD.setNb(LCDval);
}
ISR(TIMER0_OVF_vect){
}
