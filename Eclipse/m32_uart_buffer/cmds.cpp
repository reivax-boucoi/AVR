#include "cmds.h"
#include <avr/io.h>

void cmd_onLed(void) {
	if(nbParams>0){
		if(*params[0]=='?'){
			uart_transmit("\r\n\n");
			uart_transmitNb(PINC,'B');
		}else{
			PORTC|=(1<<param_int(0));
		}
	}else{
		PORTC|=(1<<PC7);
	}
	uart_prompt();
}

void cmd_offLed(void) {
	if(nbParams>0){
		PORTC&=~(1<<param_int(0));
	}else{
		PORTC&=~(1<<PC7);
	}
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
void cmd_param(void){
	uart_transmit("\r\n\nParams :");
	uart_transmitByte(48+nbParams);
	uart_transmit("\r\nFirst is : ");
	uart_transmit((char*)params[0]);
	uart_transmit("\r\nSecond is : ");
	uart_transmit((char*)params[1]);
	uart_prompt();
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

void cmd_read(void) {
	uart_transmitln("");
	if(nbParams>0){
		if(*params[0]=='?'){
			uart_transmit("Syntax : read register [D:H:B]");
		}else{
			if(nbParams>1)
			uart_transmitNb(*(uint8_t *)params[0],params[1][0]);
		}
	}
	uart_prompt();
}

void cmd_write(void) {
	if(nbParams>0){
		if(*params[0]=='?'){

		}
	}
}
