#define F_SHUTDOWN 0x01
#define F_AGC 0x02
#define F_KILL 0x04
#define F_HOLD 0x08
#define F_STREAM 0x10
#define F_2 0x20
#define F_3 0x40
#define F_4 0x80

#define EE_LCDVAL 0
#define EE_ZERO 32
#define EE_GAIN 64

#include <avr/io.h>
#include <avr/eeprom.h>
#include "include/cmds.h"
#include "include/periphs.h"
#include "include/LCD.h"

LCD myLCD;
volatile float LCDval=0.0;
volatile uint8_t gain=1;

int main(void){
	periphs_init();
	sei();
	uart_transmit_P(PSTR("\r\n\tHello World !"));
	uart_prompt();

	LCDval=eeprom_read_float(EE_LCDVAL);
	if(LCDval != LCDval)LCDval=1.23;

	while(1){
		if(GPIOR0&F_SHUTDOWN){
			uart_transmit_P(PSTR("\r\nShutting down..."));
			eeprom_update_float(EE_LCDVAL,LCDval);
			WDTCR|=(1<< WDE)|(1<<WDP0)|(1<<WDP1)|(1<<WDP2);
			while(1);
		}
		if(GPIOR0&F_KILL){
			GPIOR0&=~(0xFD);//exclude AGC // TODO check
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
	if((GPIOR0&F_HOLD)){
	}else{
		myLCD.setNb(LCDval);
	}
}
ISR(TIMER0_OVF_vect){
	if(GPIOR0&F_STREAM){
		uart_transmitNb(LCDval);
		uart_transmit_P(PSTR("\r\n\t"));
	}
}
