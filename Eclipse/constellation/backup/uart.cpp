#include "uart.h"

static volatile uint8_t  uart_buff_rx[UART_BUFFER_SIZE];
static volatile uint8_t  uart_rx_head;
static volatile uint8_t  uart_rx_tail;

void uart_init(){

	UBRR1H = 0x00;//(uint8_t)(UBBR>>8);
	UBRR1L = 0x0C;//(uint8_t)UBBR;
	UCSR1B |= (1<<RXCIE1);
	UCSR1B |= (1<<RXEN1)|(1<<TXEN1);
	UCSR1C |= (1<<UCSZ10)|(1<<UCSZ11);
	uart_rx_head=0;
	uart_rx_tail=0;
}
void uart_transmitByte(uint8_t data ){
	UCSR1B ^= ((1<<RXEN1)|(1<<RXCIE1));
	while ( !( UCSR1A & (1<<UDRE1)) );
	UDR1 = data;
	UCSR1B ^= ((1<<RXEN1)|(1<<RXCIE1));
}
void uart_transmit(char *data){
	while(*data > 0){
		uart_transmitByte(*data++);
	}
}
void uart_isr_rx(void){
	cli();
	uart_rx_head = (uart_rx_head + 1) & UART_BUFFER_MASK;
	if (uart_rx_head == uart_rx_tail) {
		// Ooops ! buffer overflow !
		uart_transmit("Receive buffer full !\r\n");
	}
	uart_buff_rx[uart_rx_head] = UDR1;
	switch (uart_buff_rx[uart_rx_head]){
	case '\n':
	case '\r':
		cmd_process();
		break;
	default:
		uart_transmitByte(uart_buff_rx[uart_rx_head]);
		break;
	}
	sei();
}
static void cmd_process(void) {
	uint8_t i=0;

}
