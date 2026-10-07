#include <avr/io.h>
#include <stdio.h>
#include "uart.h"

/* ---- stdio glue ---- */
static int uart_stdio_put(char c, FILE *stream){
    (void)stream;
    if (c == '\n') uart_putc('\r');   /* terminal-friendly CRLF */
        uart_putc(c);
    return 0;
}

static int uart_stdio_get(FILE *stream){
    (void)stream;
    char c = uart_getc();
    if (c == '\r') c = '\n';          /* terminal Enter -> '\n' for fgets/scanf */
        return (unsigned char)c;
}

static FILE uart_stream = FDEV_SETUP_STREAM(uart_stdio_put, uart_stdio_get, _FDEV_SETUP_RW);

void uart_init(void){
    UART_PORT.DIRSET = UART_TXD_bm;
    UART_PORT.DIRCLR = UART_RXD_bm;

    USARTF0.BAUDCTRLA = (uint8_t)(UART_BSEL & 0xFF);
    USARTF0.BAUDCTRLB = (uint8_t)(((UART_BSCALE&0x0F) << 4) | ((UART_BSEL >> 8) & 0x0F));
    USARTF0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;
    USARTF0.CTRLB = USART_TXEN_bm | USART_RXEN_bm;

    stdout = &uart_stream;
    stdin  = &uart_stream;
    stderr = &uart_stream;
}

void uart_putc(char c){
    while (!(USARTF0.STATUS & USART_DREIF_bm));
    USARTF0.DATA = c;
}

void uart_puts(const char *s){
    while (*s) uart_putc(*s++);
}

uint8_t uart_rx_available(void){
    return (USARTF0.STATUS & USART_RXCIF_bm) != 0;
}

char uart_getc(void){
    while (!uart_rx_available());
    return USARTF0.DATA;
}
