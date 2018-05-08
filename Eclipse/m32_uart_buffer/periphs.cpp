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
	while (uart_rx_head == uart_rx_tail);
	uart_rx_tail = (uart_rx_tail + 1) & UART_BUFFER_MASK;
	return uart_buff_rx[uart_rx_tail];
}

void uart_transmitByte(uint8_t  data){
	uart_tx_head = (uart_tx_head + 1) & UART_BUFFER_MASK;
	while (uart_tx_head == uart_tx_tail);
	uart_buff_tx[uart_tx_head] = data;
	UARTTXEN();
}

void uart_transmit(const char* data) {
	while(*data>0){
		uart_transmitByte(*data++);
	}
}

void uart_transmitln(const char* data) {
	uart_transmit(data);
	uart_transmitByte('\r');
	uart_transmitByte('\n');
}

void uart_prompt(void) {
	uart_transmit("\r\n>");

}
void uart_isr_udre(void) {
	if (uart_tx_head != uart_tx_tail) {
		uart_tx_tail = ( uart_tx_tail + 1 ) & UART_BUFFER_MASK;
		UDR = uart_buff_tx[uart_tx_tail];
	} else {
		UARTTXDIS();
	}
}


void uart_isr_rxc(void) {
	cli();
	uart_rx_head = (uart_rx_head + 1) & UART_BUFFER_MASK;
	if (uart_rx_head == uart_rx_tail) {
		// Ooops ! buffer overflow !
		uart_transmitln("Receive buffer full !");
	}
	uart_buff_rx[uart_rx_head] = UDR;
	switch (uart_buff_rx[uart_rx_head]){
	case '\n':
	case '\r':
	case '0':
		processCommand();
		break;
	case 'k':
		uart_rx_emptyBuffer();
		uart_transmit("\r\ntriggered");
		uart_prompt();
		break;
	default:
		uart_transmitByte(uart_buff_rx[uart_rx_head]);
		break;
	}
	sei();
}

uint8_t uart_receivedAvailable(void) {
	if(uart_rx_head>=uart_rx_tail)	return uart_rx_head-uart_rx_tail;
	return uart_rx_head + UART_BUFFER_SIZE - uart_rx_tail;
}

uint8_t uart_transmitAvailable(void) {
	if(uart_tx_head>=uart_rx_tail)	return uart_tx_head-uart_tx_tail;
	return uart_tx_head + UART_BUFFER_SIZE - uart_tx_tail;
}

void uart_rx_printEmptyBuffer(void) {
	while(uart_receivedAvailable()>0)uart_transmitByte(uart_receiveByte());
}
void uart_rx_emptyBuffer(void) {
	while(uart_receivedAvailable()>0)uart_receiveByte();
}
