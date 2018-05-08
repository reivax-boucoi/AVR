#include "cmds.h"
#include <avr/io.h>

void cmd_onLed(void) {
	PORTB|=(1<<PB0);
}
void cmd_offLed(void) {
	PORTB&=~(1<<PB0);
}
void cmd_info(void) {
	uart_transmitln("\r\nTerminal running version 0.2\r\nAvailable commands :");
	for(int cmd = 0; cmd < NB_COMMANDS; cmd++){
		uart_transmitByte(9);
		uart_transmitln((char*)cmd_table[cmd].str);
	}
	uart_transmitByte('>');
}

void cmd_help(void){
	uart_transmit("No help is currently available, sorry =)\r\n>");
}

void cmd_reboot(void) {
	WDTCR|=(1<< WDE)|(1<<WDP0)|(1<<WDP1)|(1<<WDP2);
	while(1);
}

void processCommand() {
	uint8_t i=0;
	if(uart_receivedAvailable()<=1){
		uart_rx_emptyBuffer();
		uart_prompt();
		return;
	}

	while(uart_receivedAvailable()>1){
		cmd_buffer[i++]=uart_receiveByte();
	}
	cmd_buffer[i]=NULLCHAR;
	int cmd;
	for (cmd = 0; cmd < NB_COMMANDS; cmd++) {//the cmd_buffer array seems fine
		if (strcmp(cmd_table[cmd].str, (char *)cmd_buffer) == 0) { // maybe the char* conversion is dubious (the strcmp function seems to work ok)
			cmd_table[cmd].fptr_t(); // the if is never true.
			return;
		}
	}
	uart_transmit("Unknown command, type \"help\" for help");
	uart_prompt();
}
