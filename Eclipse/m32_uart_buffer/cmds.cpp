#include "cmds.h"
#include <avr/io.h>

void onLed(void) {
	PORTB|=(1<<PB0);
}
void offLed(void) {
	PORTB&=~(1<<PB0);
}
void info(void) {
	uart_transmitln("\nTerminal running version 0.2\nAvailable commands :");
	for(int cmd = 0; cmd < NB_COMMANDS; cmd++){
		uart_transmitByte(9);
		uart_transmitln((char*)cmd_table[cmd].str);
	}
	uart_transmitByte('>');
}

void help(void){
	uart_transmitln("No help is currently available, sorry =)");
	uart_transmitByte('>');
}

void processCommand() {
	uint8_t i=0;
	uart_transmitByte('\n');
	if(uart_receivedAvailable()<=1){
		uart_rx_emptyBuffer();
		uart_transmitByte('>');
		return;
	}
	uart_transmitByte(uart_receivedAvailable());//debug
	while(uart_receivedAvailable()>0){
		cmd_buffer[i]=uart_receiveByte();
		uart_transmitByte(cmd_buffer[i++]);//debug
	}
	cmd_buffer[i]=NULLCHAR;

	int cmd;
	for (cmd = 0; cmd < NB_COMMANDS; cmd++) {
		if (strcmp(cmd_table[cmd].str, (char *)cmd_buffer) == 0) {
			cmd_table[cmd].fptr_t();
			return;
		}
	}
	uart_transmitln("Unknown command, type \"help\" for help ");
	uart_transmitByte('>');
}
