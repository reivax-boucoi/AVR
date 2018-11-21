#ifndef UART_H_
#define UART_H_
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Led.h"
#define BAUD 4800
#define UBBR ((F_CPU)/(BAUD*16UL)-1)
#define UART_BUFFER_SIZE 256     // 2 to 256 bytes
#define UART_BUFFER_MASK (UART_BUFFER_SIZE - 1)

void uart_init();
void uart_transmitByte(uint8_t data );
void uart_transmit(char *data);
void uart_transmitNb(uint8_t nb);
void uart_isr_rx(void);
void uart_prompt(void);
static void cmd_process(void);
#endif /* UART_H_ */
