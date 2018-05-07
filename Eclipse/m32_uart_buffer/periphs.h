#ifndef PERIPH_H
#define PERIPH_H
#include <stdio.h>
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 38400
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
#define UART_BUFFER_SIZE 256     // 2 to 256 bytes
#define UART_BUFFER_MASK (UART_BUFFER_SIZE - 1)

static uint8_t  uart_buff_rx[UART_BUFFER_SIZE];
static volatile uint8_t  uart_rx_head;
static volatile uint8_t  uart_rx_tail;
static uint8_t  uart_buff_tx[UART_BUFFER_SIZE];
static volatile uint8_t  uart_tx_head;
static volatile uint8_t  uart_tx_tail;

void uart_init(void);
uint8_t  uart_receiveByte(void);
void uart_transmitByte(uint8_t  data);
uint8_t receivedAvailable();
uint8_t transmitAvailable();

void uart_isr_udre();
void uart_isr_rxc();

#endif

