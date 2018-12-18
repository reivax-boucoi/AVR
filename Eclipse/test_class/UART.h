#ifndef UART_H_
#define UART_H_
#include <avr/io.h>
#define BAUD 9600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

class UART {

public:
	UART();
	int receive(void);
	void transmit(const char* data);
	void transmitByte(const char data);
};


#endif /* UART_H_ */
