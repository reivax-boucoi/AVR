#include <avr/io.h>
<<<<<<< HEAD
#include <avr/interrupt.h>
#include "include/uart_interpreter.h"

float LCDval=0.0;
int main(void){
	uart_init();
	DDRB|=(1<<PB6)|(1<<PB7)|(1<<PB5);
	TCCR1A|=(1<<COM1A0)|(1<<COM1A1);
	TCCR1B|=(1<<WGM12);
	OCR1A=0x00FF;
	TIMSK1|=(1<<TOIE1);
	sei();
	TCCR1B|=(1<<CS10);
	uart_transmit("\r\n\tHello World !");
	uart_prompt();
	while(1){
=======
#include <util/delay.h>
#include "include/LCD.h"

int8_t i=1;
LCD myLcd;
int main(void){
	DDRF|=(1<<PF0);
	myLcd.setDigit(0,i,DBIG);
	myLcd.setDigit(1,i,DBIG);
	myLcd.setDigit(2,i,DBIG);
	myLcd.setDigit(3,i,DBIG);
	myLcd.setDigit(4,i,DBIG);
	myLcd.setDigit(5,i,DBIG);
	myLcd.setDigit(6,i,DBIG);
	while(1){
		PORTF^=(1<<PF0);
		_delay_ms(50);

>>>>>>> 91c618586de45a19587a088cecc25a3afbae3a1d
	}
	return 1;
}

<<<<<<< HEAD
ISR(USART_RX_vect){
	uart_isr_rxc();
}
ISR(USART_UDRE_vect){
	uart_isr_udre();
}
=======
>>>>>>> 91c618586de45a19587a088cecc25a3afbae3a1d
