#include "uart_interpreter.h"

// Global variable definitions
unsigned char* params[MAXPARAM];
uint8_t nbParams;

// Static variable definitions
static volatile uint8_t  uart_buff_rx[UART_BUFFER_SIZE];
static volatile uint8_t  uart_rx_head;
static volatile uint8_t  uart_rx_tail;

static volatile uint8_t  uart_buff_tx[UART_BUFFER_SIZE];
static volatile uint8_t  uart_tx_head;
static volatile uint8_t  uart_tx_tail;

static unsigned char cmd_buffer[UART_BUFFER_SIZE];

// Static function declarations
static uint8_t uart_receivedAvailable(void);
static void uart_rx_emptyBuffer(void);

static uint8_t cmd_cmp(const char* s1,const char* s2);
static void cmd_process(void);
static void cmd_parse(void);

// Function definitions
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


void uart_transmitNb(float data) {
	char str[6];
	int8_t t = sprintf(str,"%4.2f",data);
	uart_transmit(str);
	uart_transmit("\t");
	uart_transmitNb(t,'B');
}

void uart_transmit(const char* data) {
	while(*data>0){
		uart_transmitByte(*data++);
	}
}

void uart_transmit_P(const char* data) {
	while (pgm_read_byte(data) != 0x00)
		uart_transmitByte(pgm_read_byte(data++));
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
	cli();
	if (uart_tx_head != uart_tx_tail) {
		uart_tx_tail = ( uart_tx_tail + 1 ) & UART_BUFFER_MASK;
		UDR = uart_buff_tx[uart_tx_tail];
	} else {
		UARTTXDIS();
	}
	sei();
}


void uart_isr_rxc(void) {
	cli();
	uart_rx_head = (uart_rx_head + 1) & UART_BUFFER_MASK;
	if (uart_rx_head == uart_rx_tail) {
		// Ooops ! buffer overflow !
		uart_transmit_P(PSTR("Receive buffer full !\r\n"));
	}
	uart_buff_rx[uart_rx_head] = UDR;
	switch (uart_buff_rx[uart_rx_head]){
	case '\n':
	case '\r':
		cmd_process();
		break;

	case KILL:
		uart_rx_emptyBuffer();
		uart_transmit_P(PSTR("\n\rKilled all running processes."));
		uart_transmit_P(PSTR("\r\nPress 'SHIFT+K' to resume."));
		uart_prompt();
		break;

	default:
		uart_transmitByte(uart_buff_rx[uart_rx_head]);
		break;
	}
	sei();
}

static uint8_t uart_receivedAvailable(void) {
	if(uart_rx_head>=uart_rx_tail)	return uart_rx_head-uart_rx_tail;
	return uart_rx_head + UART_BUFFER_SIZE - uart_rx_tail;
}


static void uart_rx_emptyBuffer(void) {
	while(uart_receivedAvailable()>0)uart_receiveByte();
}

static uint8_t cmd_cmp(const char* cmd, const char* entry) {
	uint8_t c=0;
	while(cmd[c]!=NULLCHAR && entry[c]!=NULLCHAR){
		if(cmd[c]!=entry[c])return 0;
		c++;
	}
	if(entry[c]==NULLCHAR)return 1;
	return 0;
}


static void cmd_process(void) {
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
	cmd_parse();
	for (uint8_t cmd = 0; cmd < NB_COMMANDS; cmd++) {

		if (cmd_cmp(cmd_table[cmd].str, (char *)cmd_buffer)) {
			cmd_table[cmd].fptr_t();
			return;
		}
	}
	uart_transmit_P(PSTR("\r\nUnknown command, type \"help\" for help"));
	uart_prompt();
}

static void cmd_parse(void) {//TODO check
	int l;
	nbParams = 0;
	for (l=0;l<MAXPARAM;l++)params[l] = NULL;

	for (l = 0; cmd_buffer[l] != NULLCHAR; l++) {
		if(nbParams>=MAXPARAM){
			uart_transmit_P(PSTR("Max number of parameters exceeded !\r\n"));
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
float param_float(uint8_t nb){
	return atof((char*)params[nb]);
}

