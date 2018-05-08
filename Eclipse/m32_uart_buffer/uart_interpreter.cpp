#include "uart_interpreter.h"

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


uint8_t cmp_cmd(const char* cmd, const char* entry) {
	uint8_t c=0;
	while(cmd[c]!=NULLCHAR && entry[c]!=NULLCHAR){
		if(cmd[c]!=entry[c])return 0;
		c++;
	}
	if(cmd[c]==NULLCHAR)return 1;
	return 0;
}


void processCommand() {
	uint8_t i=0;
	if(uart_receivedAvailable()<=1){
		uart_rx_emptyBuffer();
		uart_prompt();
		return;
	}
	while(uart_receivedAvailable()>0){
		cmd_buffer[i++]=uart_receiveByte();
	}
	cmd_buffer[i-1]=NULLCHAR;
	for (uint8_t cmd = 0; cmd < NB_COMMANDS; cmd++) {

		if (cmp_cmd(cmd_table[cmd].str, (char *)cmd_buffer)) {
			cmd_table[cmd].fptr_t();
			return;
		}
	}
	uart_transmit("\r\nUnknown command, type \"help\" for help");
	uart_prompt();
}
