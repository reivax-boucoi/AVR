#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "periphs.h"
#define LTADDR 0x14

uint16_t adcExt_read(){
	uint16_t data;
	i2c_start((LTADDR<<1)+I2C_READ);
	data=i2c_read_ack()<<8;
	data|=i2c_read_nack();
	i2c_stop();
	return data;
}
float temp_read(){
	uint16_t data=0;
	i2c_start((SLA<<1)+I2C_WRITE);
	i2c_write(TEMPREG);
	i2c_start((SLA<<1)+I2C_READ);
	data=i2c_read_ack()<<4;
	data|=i2c_read_nack()>>4;
	i2c_stop();
	float temp =(data & 0x7FFF)/16.0;
	if(data & 0x800)temp=-temp;
	return temp;
}
int main(void){
	DDRB |= (1 << PB0) | (1<<PB1);
	PORTB |= (1<<PB0) | (0<PB1);
	DDRC |=(1<<PA7);
	i2c_init();
	uart_init();
	adc_init();
	sei();
	//temp_init();
	while(1){
		float temp =0.0;// temp_read();
		uint16_t adc=adcExt_read();
		if(temp>25.0)PORTC^=(1<<PA7);
		char str[40] = {0};
		sprintf(str, "T = %04.3f°C, LC2451 = %i\r\n",temp,adc);
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
