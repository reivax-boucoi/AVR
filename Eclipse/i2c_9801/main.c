#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "periphs.h"
#define SLA 0b1001111
#define TEMPREG 0x00
#define CONFREG 0x01
#define HYSTREG 0x02
#define LIMREG 0x03
#define TEMPMIN 0x18F0 // 24.5°C
#define TEMPMAX 0x1940 // 25°C

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
	i2c_start((SLA<<1)+I2C_WRITE);
	i2c_write(CONFREG);
	i2c_write(0b01100000);
	i2c_stop();
	_delay_ms(1000);
	i2c_start((SLA<<1)+I2C_WRITE);
	i2c_write(HYSTREG);
	i2c_write(TEMPMIN >>8);
	i2c_write(TEMPMIN & 0xFF);
	i2c_stop();
	_delay_ms(1000);
	i2c_start((SLA<<1)+I2C_WRITE);
	i2c_write(LIMREG);
	i2c_write(TEMPMAX >>8);
	i2c_write(TEMPMAX & 0xFF);
	i2c_stop();
	_delay_ms(1000);
	uart_transmitNow("Configured temp sensor\r\n");
	while(1){
		uint16_t data=0;
		int8_t test;
		i2c_start((SLA<<1)+I2C_WRITE);
		i2c_write(TEMPREG);
		i2c_start((SLA<<1)+I2C_READ);
		test=i2c_read_ack();
		data=test<<4;
		data|=i2c_read_nack()>>4;
		i2c_stop();
		float temp =(data & 0x7FFF)/16.0;
		if(data & 0x800)temp=-temp;
		if(temp>25.0)PORTC^=(1<<PA7);
		char str[40] = {0};
		sprintf(str, "T = %04.3lf°C, raw = %04X\r\n",temp,data);
		uart_transmitNow(str);
		PORTB^=(1<<PB0);
		_delay_ms(1000);
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
ISR (USART_UDRE_vect ){
	uart_isr_udre();
}
