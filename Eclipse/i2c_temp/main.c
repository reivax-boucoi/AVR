#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "periphs.h"
#define SLA 0x40
#define TEMPREG 0xE3


float calc(uint16_t v){
	return ((175.72*v)/65536)-46.85;
}
int main(void){
	DDRB |= (1 << PB0) | (1<<PB1);
	PORTB |= (1<<PB0) | (0<PB1);
	DDRC |=(1<<PA7);
	i2c_init();
	uart_init();
	adc_init();
	sei();
	while(1){
		uint16_t data=0;
		i2c_start((SLA<<1)+I2C_WRITE);
		i2c_write(TEMPREG);
		i2c_start((SLA<<1)+I2C_READ);
		data=(i2c_read_ack()<<8);
		data|=i2c_read_nack();
		i2c_stop();
		float temp = calc(data);
		if(temp>22.0)PORTC^=(1<<PA7);
		char str[10] = {0};
		sprintf(str, "T = %08.6lf°C, t = %i\r\n",temp,tempo);
		uart_transmitBuff(str,strlen(str));
		PORTB^=(1<<PB0);
		_delay_ms(tempo);
	}
	return 0;
}
ISR (ADC_vect ){
	adc_read();
}
ISR (TWI_vect ){
}
ISR (USART_RXC_vect ){
	uart_isr_rx();
}
ISR (USART_TXC_vect ){
	uart_isr_tx();
}
ISR (USART_UDRE_vect ){
	uart_isr_udre();
}
