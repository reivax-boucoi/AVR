#define F_SHUTDOWN 0x01
#define F_AGC 0x02
#define F_KILL 0x04
#define F_HOLD 0x08
#define F_1 0x10
#define F_2 0x20
#define F_3 0x40
#define F_4 0x80

#include <avr/io.h>
#include <avr/cpufunc.h>
#include "include/cmds.h"
#include "include/periphs.h"
#include "include/LCD.h"

LCD myLCD;
volatile float LCDval=0.0;
volatile uint8_t FLAG=0x00;

int main(void){
	periphs_init();
	for(uint16_t i=0xFFFF;i>0;i--)_NOP();
	sei();
	uart_transmit_P(PSTR("\r\n\tHello World !"));
	uart_prompt();
	while(1){
		if(FLAG&F_SHUTDOWN){
			uart_transmit_P(PSTR("Shutting down..."));
			WDTCR|=(1<< WDE)|(1<<WDP0)|(1<<WDP1)|(1<<WDP2);
			while(1);
		}
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
