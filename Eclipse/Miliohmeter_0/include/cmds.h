#ifndef CMDS_H_
#define CMDS_H_

#include "uart_interpreter.h"
#include <avr/pgmspace.h>
typedef struct {
	void (*fptr_t)(void);
	const char * str;
} cmd_t;

void cmd_display(void);
void cmd_gain(void);
void cmd_info(void);
void cmd_help(void);
void cmd_clear(void);
void cmd_reboot(void);

static const cmd_t cmd_table[] = {
		{ cmd_display, "display"},
		{ cmd_gain, "gain"},
		{ cmd_info, "info"},
		{ cmd_help, "help"},
		{ cmd_clear, "clear"},
		{ cmd_reboot, "reboot"}
};

#define NB_COMMANDS (uint8_t)(sizeof(cmd_table) / sizeof(cmd_table[0]))

#endif /* CMDS_H_ */
