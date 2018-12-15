#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "UART.h"

int main(void){
	UART uart;
	while(1){
		_delay_ms(500);
		uart.transmit("Hello world!\r\n");
	}
	return 0;
}
