#include "cmds.h"
#include <avr/io.h>

void onLed(void) {
	PORTB|=(1<<PB0);
}
void offLed(void) {
	PORTB&=~(1<<PB0);
}
void info(void) {
	uart_transmitln((char*)"Terminal running version n°0.1\nAvailable commands :");
	for(int cmd = 0; cmd < N_COMMANDS; cmd++){
		uart_transmitln((char*)cmd_table[cmd].str);
	}
	uart_transmitByte('>');
}

void ProcessCommand() {
	uint8_t i=0;
	if(uart_receivedAvailable()<=1){
		uart_transmitln((char*)"not enough content !");
		uart_transmitByte('>');
	}
	while(uart_receivedAvailable()>1)cmd_buffer[i++]=uart_receiveByte();
	cmd_buffer[i]=NULLCHAR;

	int cmd;
	for (cmd = 0; cmd < N_COMMANDS; cmd++) {
		if (strcmp(cmd_table[cmd].str, (char *)cmd_buffer) == 0) {
			cmd_table[cmd].fptr_t();
			return;
		}
	}
	uart_transmitln((char*)"Unknown command, type \"help\" for help ");
	uart_transmitByte('>');
}
