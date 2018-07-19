<<<<<<< HEAD
#ifndef UART_INTERPRETER_H_
#define UART_INTERPRETER_H_

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "cmds.h"

#define BAUD 4800
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

#define MAXPARAM 5

#ifndef NULLCHAR
#define NULLCHAR '\0'
#endif

#define KILL 27//'K'

#define UART_BUFFER_SIZE 128     // 2 to 256 bytes
#define UART_BUFFER_MASK (UART_BUFFER_SIZE - 1)

#define UARTTXEN()	UCSR0B |= (1<<UDRIE0)
#define	UARTTXDIS() UCSR0B &= ~(1<<UDRIE0)

extern unsigned char* params[MAXPARAM];
extern uint8_t nbParams;

// uart buffering and hardware functions
void uart_init(void);
uint8_t uart_receiveByte(void);
void uart_transmitByte(uint8_t data);
void uart_transmit(const char *data);
void uart_transmit_P(const char *data);
void uart_transmitNb(uint8_t data, uint8_t mode);
void uart_transmitNb(float data);
void uart_prompt(void);

// uart isr
void uart_isr_udre(void);
void uart_isr_rxc(void);

// uart command interpreter handling
bool cmd_cmp(const char* s1,const char* s2);
uint8_t param_int(uint8_t nb);
float param_float(uint8_t nb);

#endif /* UART_INTERPRETER_H_  */

=======
#ifndef UART_INTERPRETER_H_
#define UART_INTERPRETER_H_

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "cmds.h"

#define BAUD 38400
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

#define MAXPARAM 5

#ifndef NULLCHAR
#define NULLCHAR '\0'
#endif

#define KILL 27//'K'

#define UART_BUFFER_SIZE 128     // 2 to 256 bytes
#define UART_BUFFER_MASK (UART_BUFFER_SIZE - 1)

#define UARTTXEN()	UCSR0B |= (1<<UDRIE0)
#define	UARTTXDIS() UCSR0B &= ~(1<<UDRIE0)

extern unsigned char* params[MAXPARAM];
extern uint8_t nbParams;

// uart buffering and hardware functions
void uart_init(void);
uint8_t uart_receiveByte(void);
void uart_transmitByte(uint8_t data);
void uart_transmit(const char *data);
void uart_transmit_P(const char *data);
void uart_transmitNb(uint8_t data, uint8_t mode);
void uart_transmitNb(float data);
void uart_prompt(void);

// uart isr
void uart_isr_udre(void);
void uart_isr_rxc(void);

// uart command interpreter handling
bool cmd_cmp(const char* s1,const char* s2);
uint8_t param_int(uint8_t nb);
float param_float(uint8_t nb);

#endif /* UART_INTERPRETER_H_  */

>>>>>>> 9d3b0f646835cac570ebc3fe71356051f60b58ad
