#ifndef UART_H_
#define UART_H_
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define BAUD 4800

void uart_init();
void uart_transmitByte(uint8_t data );
void uart_transmit(char *data);
void uart_transmitNb(uint16_t nb);
void uart_prompt(void);
#endif /* UART_H_ */
