#include "uart_interpreter.h"

unsigned char* params[MAXPARAM];
uint8_t nbParams;

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

void uart_transmitNb(uint8_t data,uint8_t mode) {
	char str[]="0b00000000";
	switch(mode){
	case 'H':
		sprintf(str,"0x%X",data);
		break;
	case 'B':
		str[0]='0';
		str[1]='b';
		for(uint8_t i=2;i<10;i++){
			if(data&(1<<(9-i)))str[i]='1';
		}
		break;
	case 'D':
	default:
		sprintf(str,"%u",data);
		break;
	}
	uart_transmit(str);
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

void uart_transmitnl(const char* data) {
	uart_transmitByte('\r');
	uart_transmitByte('\n');
	uart_transmit(data);
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
		cmd_process();
		break;

	case KILL:
		uart_rx_emptyBuffer();
		uart_transmitnl("Killed all running processes.");
		uart_transmitnl("Press 'SHIFT+K' to resume.");
		uart_prompt();
		break;

	case PREV:
		uart_rx_loadBuffer(last_cmd);
		uart_rx_printBuffer();//TODO redraw command
		break;

	case BACKSPACE:
		uart_buff_rx_removeLast();// TODO fix this
		uart_transmitByte('\b');
		uart_transmitByte(' ');
		uart_transmitByte('\b');
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
void uart_rx_printBuffer(void) {
	uart_transmit("");
	uint8_t i=uart_rx_tail;
	while(i!=uart_rx_head){
		uart_transmitByte(uart_buff_rx[i]);
		i = (i+1) & UART_BUFFER_MASK;
	}
}
void uart_rx_emptyBuffer(void) {
	while(uart_receivedAvailable()>0)uart_receiveByte();
}
void uart_rx_loadBuffer(const unsigned char* s) {
	for(uint8_t i=0, uart_rx_head=uart_rx_tail;s[i]!=NULLCHAR;i++){
		uart_buff_rx[uart_rx_head]=s[i];
		uart_rx_head= (uart_rx_head+1) & UART_BUFFER_MASK;
	}
}

void uart_buff_rx_removeLast(void){
	if(uart_rx_head!=uart_rx_tail){
		if(uart_rx_head==0){
			uart_rx_head=UART_BUFFER_MASK;
		}else{
			uart_rx_head=uart_rx_head-1;
		}
	}
}

uint8_t cmd_cmp(const char* cmd, const char* entry) {
	uint8_t c=0;
	while(cmd[c]!=NULLCHAR && entry[c]!=NULLCHAR){
		if(cmd[c]!=entry[c])return 0;
		c++;
	}
	if(entry[c]==NULLCHAR)return 1;
	return 0;
}


void cmd_process(void) {
	uint8_t i=0;
	if(uart_receivedAvailable()<=1){
		uart_rx_emptyBuffer();
		uart_prompt();
		return;
	}
	memcpy(last_cmd,cmd_buffer,sizeof(cmd_buffer));
	while(uart_receivedAvailable()>0){
		cmd_buffer[i++]=uart_receiveByte();
	}
	cmd_buffer[i-1]=NULLCHAR;
	cmd_parse();
	for (uint8_t cmd = 0; cmd < NB_COMMANDS; cmd++) {

		if (cmd_cmp(cmd_table[cmd].str, (char *)cmd_buffer)) {
			if(nbParams>0 && *params[0]=='?'){
				uart_transmitnl(cmd_table[cmd].descr);
				uart_prompt();
			}else{
				cmd_table[cmd].fptr_t();
			}
			return;
		}
	}
	uart_transmitnl("Unknown command, type \"help\" for help");
	uart_prompt();
}

void cmd_parse(void) {//TODO check
	int l;
	nbParams = 0;
	for (l=0;l<MAXPARAM;l++)params[l] = NULL;

	for (l = 0; cmd_buffer[l] != NULLCHAR; l++) {
		if(nbParams>=MAXPARAM){
			uart_transmitln("Max number of parameters exceeded !");
			break;
		}
		if (cmd_buffer[l] == ' ') {
			cmd_buffer[l] = NULLCHAR;
			params[nbParams++] = &cmd_buffer[l] + 1;
		}
	}

}

uint8_t param_int(uint8_t nb){
	return atoi((char*)params[nb]);
}


