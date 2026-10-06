#ifndef UART_H
#define UART_H

#include <stdint.h>

/* ---- Pin assignment (USARTF0) ---- */
#define UART_PORT       PORTF
#define UART_TXD_bm     PIN3_bm   /* PF3 = TXD */
#define UART_RXD_bm     PIN2_bm   /* PF2 = RXD */

/* ---- UART configuration ---- */
#define UART_BAUD       9600UL
#define UART_BSEL       207       /* BSCALE=0, F_CPU=32MHz -> ~9615 baud (0.16% error) */
#define UART_BSCALE     0

/* Initializes USARTF0 and binds stdin/stdout/stderr to it,
 * so printf()/scanf()/getchar()/fgets() work. */
void uart_init(void);

/* Raw access (no newline translation) */
void uart_putc(char c);
void uart_puts(const char *s);
char uart_getc(void);          /* blocking */
uint8_t uart_rx_available(void);

#endif
