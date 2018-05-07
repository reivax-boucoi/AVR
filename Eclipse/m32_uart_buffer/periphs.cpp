#include "periphs.h"


void uart_init(void){
	UBRRH = (BAUDRATE>>8);
	UBRRL = BAUDRATE;
	UCSRB = ((1<<RXEN) | (1<<TXEN) | (1<<RXCIE));
	uint8_t  x = 0;
	uart_rx_tail = x;
	uart_rx_head = x;
	uart_tx_tail = x;
	uart_tx_head = x;
}


uint8_t  uart_receiveByte(void){
	while (uart_rx_head == uart_rx_tail); // wait until at least 1 elt is in buffer
	uart_rx_tail = (uart_rx_tail + 1) & UART_BUFFER_MASK; // increment tail
	return uart_buff_rx[uart_rx_tail]; //return value
}

void uart_transmitByte(uint8_t  data){
	uart_tx_head = (uart_tx_head + 1) & UART_BUFFER_MASK;
	while (uart_tx_head == uart_tx_tail);
	uart_buff_tx[uart_tx_head] = data;
	UCSRB |= (1<<UDRIE);
}

void uart_isr_udre() {
	if (uart_tx_head != uart_tx_tail) {
		uart_tx_tail = ( uart_tx_tail + 1 ) & UART_BUFFER_MASK;
		UDR = uart_buff_tx[uart_tx_tail];
	} else {
		UCSRB &= ~(1<<UDRIE);
	}
}

void uart_isr_rxc() {
	uart_rx_head = (uart_rx_head + 1) & UART_BUFFER_MASK;
	if (uart_rx_head == uart_rx_tail) {
		// Ooops ! buffer overflow !
	}
	uart_buff_rx[uart_rx_head] = UDR;
}

uint8_t receivedAvailable() {
	if(uart_rx_head>=uart_rx_tail)	return uart_rx_head-uart_rx_tail;
	return uart_rx_head + UART_BUFFER_SIZE - uart_rx_tail;
}

uint8_t transmitAvailable() {
	if(uart_tx_head>=uart_rx_tail)	return uart_tx_head-uart_tx_tail;
	return uart_tx_head + UART_BUFFER_SIZE - uart_tx_tail;
}
