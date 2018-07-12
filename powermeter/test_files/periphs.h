#ifndef PERIPHS_H
#define PERIPHS_H

#include <avr/io.h>

#define BAUD 4800
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

#define CS PINB2

// ADC config:xxxxSSx1 0
#define VCH0 0b00001100
#define ICH0 0b00001001 // CH2ref-/3sig+ (diff)



void uart_init (void);
void uart_transmit (uint8_t data);
void uart_transmitMult(char *Data);
uint8_t uart_recieve (void);
void spi_masterInit(void);
uint8_t spi_rxtx(uint8_t data);
uint16_t adc_v(void);
int16_t adc_i(void);

#endif