#ifndef CMDS_H_
#define CMDS_H_

#include "uart_interpreter.h"

typedef struct {
	void (*fptr_t)(void);
	const char * str;
	const char * descr;
} cmd_t;

void cmd_onLed(void);
void cmd_offLed(void);
void cmd_info(void);
void cmd_read(void);
void cmd_write(void);
void cmd_help(void);
void cmd_clear(void);
void cmd_param(void);
void cmd_reboot(void);

static const cmd_t cmd_table[] = {
		{ cmd_onLed, "onLed","Syntax : onLed [BIT] default:7" },
		{ cmd_offLed, "offLed","Syntax : offLed [BIT] default:7" },
		{ cmd_info, "info","Prints version number and command list" },
		{ cmd_read, "read","Echoes register.\r\nSyntax : read [register] [B:H:D] default:Binary" },
		{ cmd_write, "write","Writes register.\r\nSyntax : write [register] [B:H:D] default:Binary"},
		{ cmd_help, "help","Displays available help, use help [cmd] to get specific command help" },
		{ cmd_clear, "clear","Clears the screen" },
		{ cmd_param, "param","Displays command parameters in a list" },
		{cmd_reboot, "reboot","Reboot the target, immediate, no arguments" }
};
static const uint8_t NB_COMMANDS = sizeof (cmd_table)/ sizeof (cmd_t);


#endif /* CMDS_H_ */
