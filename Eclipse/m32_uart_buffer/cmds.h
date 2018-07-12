#ifndef CMDS_H_
#define CMDS_H_

#include "uart_interpreter.h"

typedef struct {
	void (*fptr_t)(void);
	const char * str;
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
//truc
static const cmd_t cmd_table[] = {
		{ cmd_onLed, "onLed"},
		{ cmd_offLed, "offLed"},
		{ cmd_info, "info"},
		{ cmd_read, "read"},
		{ cmd_write, "write"},
		{ cmd_help, "help"},
		{ cmd_clear, "clear"},
		{ cmd_param, "param"},
		{ cmd_reboot, "reboot"}
};

#define NB_COMMANDS (uint8_t)(sizeof(cmd_table) / sizeof(cmd_table[0]))

#endif /* CMDS_H_ */
