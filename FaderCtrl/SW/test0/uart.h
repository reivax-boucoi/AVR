#ifndef UART_H
#define UART_H

#include <stdint.h>

/* ---- Pin assignment (USARTF0) ---- */
#define UART_PORT       PORTF
#define UART_TXD_bm     PIN3_bm   /* PF3 = TXD */
#define UART_RXD_bm     PIN2_bm   /* PF2 = RXD */

/* ---- UART configuration ---- */
#define UART_BAUD       115200UL
#define UART_BSEL       1047
#define UART_BSCALE     (-6)

//For BSCALE=0, Baudrate=F_CPU/(16*(BSEL+1))
// BSEL=207,BSCALE= 0 => Baudrate=9615
//BSEL=1047,BSCALE=-6 => Baudrate=115211

/* Initializes USARTF0 and binds stdin/stdout/stderr to it,
 * so printf()/scanf()/getchar()/fgets() work. */
void uart_init(void);

/* Raw access (no newline translation) */
void uart_putc(char c);
void uart_puts(const char *s);
char uart_getc(void);          /* blocking */
uint8_t uart_rx_available(void);

#endif
