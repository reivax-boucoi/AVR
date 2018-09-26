#ifndef PERIPH_H
#define PERIPH_H
#include <stdio.h>
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#define BUFFERSIZE 200
typedef struct {
	uint8_t head;
	uint8_t tail;
	char data[BUFFERSIZE];
}T_uart_buffer;

void uart_buffer_init(T_uart_buffer *b);
uint8_t uart_buffer_free(T_uart_buffer *b);
uint8_t uart_buffer_length(T_uart_buffer *b);
char uart_buffer_readByte(T_uart_buffer *b);
//void uart_buffer_writeByte(T_uart_buffer *b,char data);
//uint8_t uart_buffer_read(T_uart_buffer *b, char *data, uint8_t l);
uint8_t uart_buffer_write(T_uart_buffer *b,char *data,uint8_t l);

#define UARTTXEN()	UCSRB |= (1<<UDRIE)
#define	UARTTXDIS() UCSRB &= ~(1<<UDRIE)

#define BAUD 38400
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
extern volatile uint16_t tempo;
void uart_init (void);
//void uart_transmit(char data);
//void uart_transmitNow(char *data);
void uart_transmitBuff(char *data,uint8_t l);
//unsigned char uart_recieve (void);
//void uart_isr_rx();
void uart_isr_udre();

void adc_init(void);
uint8_t adc_read(void);

#endif
