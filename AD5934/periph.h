#include <avr/io.h>
#define BAUD 115200                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR
#define I2C_ADDR 0x0D

void uart_init (void);
void uart_transmitByte(uint8_t data);
void uart_transmit(int8_t *data);
uint8_t uart_recieve(void);
