<<<<<<< HEAD
#ifndef UART_INTERPRET_H
#define UART_INTERPRET_H
#include <stdio.h>
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "cmds.h"

#define BAUD 38400
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

#ifndef NULLCHAR
#define NULLCHAR '\0'
#endif

#define KILL 'k'
#define PREV '&'
#define BACKSPACE 127

#define UART_BUFFER_SIZE 256     // 2 to 256 bytes
#define UART_BUFFER_MASK (UART_BUFFER_SIZE - 1)
#define UARTTXEN()	UCSRB |= (1<<UDRIE)
#define	UARTTXDIS() UCSRB &= ~(1<<UDRIE)

static  uint8_t  uart_buff_rx[UART_BUFFER_SIZE];
static volatile uint8_t  uart_rx_head;
static volatile uint8_t  uart_rx_tail;
static uint8_t  uart_buff_tx[UART_BUFFER_SIZE];
static volatile uint8_t  uart_tx_head;
static volatile uint8_t  uart_tx_tail;

// uart buffering and hardware functions
void uart_init(void);
uint8_t  uart_receiveByte(void);
void uart_transmitByte(uint8_t  data);
void uart_transmit(const char  *data);
void uart_transmitln(const char  *data);
uint8_t uart_receivedAvailable(void);
uint8_t uart_transmitAvailable(void);
void uart_buff_rx_removeLast(void);
void uart_rx_emptyBuffer(void);
void uart_rx_loadBuffer(const unsigned char* s);
void uart_rx_printBuffer(void);
void uart_rx_printEmptyBuffer(void);
void uart_prompt(void);
void uart_isr_udre(void);
void uart_isr_rxc(void);

//uart command interpreter handling

#define MAXPARAM 5
volatile unsigned char* params[MAXPARAM];
volatile uint8_t nbParams;
static unsigned char cmd_buffer[256];
static unsigned char last_cmd[256];

uint8_t cmd_cmp(const char* s1,const char* s2);
void cmd_process(void);
void cmd_parse(void);

#endif

=======
#ifndef UART_INTERPRET_H
#define UART_INTERPRET_H
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "cmds.h"

#define BAUD 38400
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

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

static  uint8_t  uart_buff_rx[UART_BUFFER_SIZE];
static volatile uint8_t  uart_rx_head;
static volatile uint8_t  uart_rx_tail;
static uint8_t  uart_buff_tx[UART_BUFFER_SIZE];
static volatile uint8_t  uart_tx_head;
static volatile uint8_t  uart_tx_tail;

// uart buffering and hardware functions
void uart_init(void);
uint8_t  uart_receiveByte(void);
void uart_transmitByte(uint8_t  data);
void uart_transmit(const char  *data);
void uart_transmitNb(uint8_t data, uint8_t mode);
void uart_transmitln(const char  *data);
uint8_t uart_receivedAvailable(void);
uint8_t uart_transmitAvailable(void);
void uart_buff_rx_removeLast(void);
void uart_rx_emptyBuffer(void);
void uart_rx_loadBuffer(const unsigned char* s);
void uart_rx_printBuffer(void);
void uart_rx_printEmptyBuffer(void);
void uart_prompt(void);
void uart_isr_udre(void);
void uart_isr_rxc(void);

//uart command interpreter handling

#define MAXPARAM 5
extern unsigned char* params[MAXPARAM];
extern uint8_t nbParams;
static unsigned char cmd_buffer[UART_BUFFER_SIZE];
static unsigned char last_cmd[UART_BUFFER_SIZE];

uint8_t cmd_cmp(const char* s1,const char* s2);
void cmd_process(void);
void cmd_parse(void);
uint8_t param_int(uint8_t nb);

#endif

>>>>>>> 1763f12d82ef3dec00ac88e2cead4740fc16ee53
