#ifndef UART_H_
#define UART_H_
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define BAUD 4800

#define BINTODECPATT "%c%c%c%c%c%c%c%c"
#define BINTODEC(uint8_t)  \
  (uint8_t & 0x80 ? '1' : '0'), \
  (uint8_t & 0x40 ? '1' : '0'), \
  (uint8_t & 0x20 ? '1' : '0'), \
  (uint8_t & 0x10 ? '1' : '0'), \
  (uint8_t & 0x08 ? '1' : '0'), \
  (uint8_t & 0x04 ? '1' : '0'), \
  (uint8_t & 0x02 ? '1' : '0'), \
  (uint8_t & 0x01 ? '1' : '0')

void uart_init();
void uart_transmitByte(uint8_t data );
void uart_transmit(char *data);
void uart_transmitNb(uint16_t nb);
void uart_transmitNb(uint16_t nb, uint8_t mode);
void uart_prompt(void);
#endif /* UART_H_ */
