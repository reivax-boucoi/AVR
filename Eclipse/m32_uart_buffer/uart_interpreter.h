#ifndef UART_INTERPRETER_H_
#define UART_INTERPRETER_H_

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "cmds.h"

#define BAUD 38400
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

#define MAXPARAM 5

#ifndef NULLCHAR
#define NULLCHAR '\0'
#endif

#define KILL 'K'
#define PREV '&'
#define BACKSPACE 127

#define UART_BUFFER_SIZE 256     // 2 to 256 bytes
#define UART_BUFFER_MASK (UART_BUFFER_SIZE - 1)
#define UARTTXEN()	UCSRB |= (1<<UDRIE)
#define	UARTTXDIS() UCSRB &= ~(1<<UDRIE)

extern unsigned char* params[MAXPARAM];
extern uint8_t nbParams;

// uart buffering and hardware functions
void uart_init(void);
uint8_t uart_receiveByte(void);
void uart_transmitByte(uint8_t data);
void uart_transmit(const char *data);
void uart_transmitNb(uint8_t data, uint8_t mode);
void uart_transmitln(const char *data);
void uart_transmitnl(const char *data);
void uart_prompt(void);

// uart isr
void uart_isr_udre(void);
void uart_isr_rxc(void);

// uart command interpreter handling
uint8_t param_int(uint8_t nb);

#endif /* UART_INTERPRETER_H_  */

