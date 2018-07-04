#include "cmds.h"

//TODO
extern float LCDval;

void cmd_display(void) {
	if(nbParams>0){
		LCDval = param_float(0);
	}else{
		uart_transmit("\t\r\n");
		uart_transmitNb(LCDval);
	}
	uart_prompt();
}

void cmd_gain(void) {
	if(nbParams>0){
		PORTC&=~(1<<param_int(0));
	}else{
		PORTC&=~(1<<PC7);
	}
	uart_prompt();
}

void cmd_info(void) {
	uart_transmit_P(PSTR("\r\nTerminal running version 0.3\r\nAvailable commands :\r\n"));
	for(int cmd = 0; cmd < NB_COMMANDS; cmd++){
		uart_transmit("\r\n\t");
		uart_transmit((char*)cmd_table[cmd].str);
	}
	uart_prompt();
}

void cmd_help(void){
	uart_transmit_P(PSTR("\r\nType [cmd] ? to get specific command help.\r\nType \"info\" for a command list"));
	uart_prompt();
}

void cmd_reboot(void) {
	//FLAG|=F_SHUTDOWN;
}

void cmd_clear(void) {
	for(uint8_t i=0;i<50;i++){
		uart_transmit("\r\n");
	}
	uart_prompt();
}