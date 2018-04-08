#ifndef PERIPH_H
#define PERIPH_H
#include <stdio.h>
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/twi.h>

//I2C
#define I2C_READ 0x01
#define I2C_WRITE 0x00
#define F_SCL 400000UL
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16 ) / 2)

void i2c_init(void);
uint8_t i2c_start(uint8_t address);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);
void i2c_stop(void);

//UART
#define BAUD 38400
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
extern volatile uint16_t tempo;
void uart_init (void);
void uart_transmit(char data);
void uart_transmitNow(char *data);
unsigned char uart_recieve (void);
void uart_isr_rx();
void uart_isr_udre();

//INTERNAL ADC
void adc_init(void);
uint8_t adc_read(void);

//I2C TEMP SENSOR
#define SLA 0b1001111
#define TEMPREG 0x00
#define CONFREG 0x01
void temp_init();
//float temp_read();

#endif
