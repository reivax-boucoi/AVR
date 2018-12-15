#ifndef UART_H_
#define UART_H_
#include <avr/io.h>
#define BAUD 115200                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

class UART {
public:
	UART(void);
	uint8_t recieve(void);
	void transmitByte(char data);
	void transmit(char *data);
};

#endif /* UART_H_ */
