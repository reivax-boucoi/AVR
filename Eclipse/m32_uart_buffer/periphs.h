#ifndef PERIPH_H
#define PERIPH_H
#include <stdio.h>
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "cmds.h"


#define BAUD 38400
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

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
void uart_rx_emptyBuffer(void);
void uart_rx_printEmptyBuffer(void);
void uart_prompt(void);
void uart_isr_udre(void);
void uart_isr_rxc(void);


uint8_t cmp_cmd(const char* s1,const char* s2);
void processCommand();

#endif

