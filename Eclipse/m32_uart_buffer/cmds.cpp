#include "cmds.h"
#include <avr/io.h>

void cmd_onLed(void) {
	PORTB|=(1<<PB0);
	uart_prompt();
}
void cmd_offLed(void) {
	PORTB&=~(1<<PB0);
	uart_prompt();
}
void cmd_info(void) {
	uart_transmitln("\r\nTerminal running version 0.3\r\nAvailable commands :");
	for(int cmd = 0; cmd < NB_COMMANDS; cmd++){
		uart_transmitByte(9);
		uart_transmitln((char*)cmd_table[cmd].str);
	}
	uart_transmitByte('>');
}

void cmd_help(void){
	uart_transmit("\r\nNo help is currently available, sorry =)");
	cmd_info();
}

void cmd_reboot(void) {
	WDTCR|=(1<< WDE)|(1<<WDP0)|(1<<WDP1)|(1<<WDP2);
	while(1);
}

void cmd_clear(void) {
	for(uint8_t i=0;i<50;i++){
		uart_transmitln("");
	}
	uart_prompt();
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
