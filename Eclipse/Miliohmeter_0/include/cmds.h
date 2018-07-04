#ifndef CMDS_H_
#define CMDS_H_

#include "uart_interpreter.h"
#include <avr/pgmspace.h>

typedef struct {
	void (*fptr_t)(void);
	const char * str;
	const char * descr;// PROGMEM; TODO
} cmd_t;

void cmd_display(void);
void cmd_gain(void);
void cmd_info(void);
void cmd_help(void);
void cmd_clear(void);
void cmd_reboot(void);

static const cmd_t cmd_table[] = {

		{ cmd_display, "display","Returns display value, sets display value with first argument"},
		{ cmd_gain,    "gain",   "Returns current gain, set gain or disables AGC : on/off"},
		{ cmd_info,    "info",   "Returns version number and command list" },
		{ cmd_help,    "help",   "Displays available help, use help [cmd] to get specific command help" },
		{ cmd_clear,   "clear",  "Clears the screen" },
		{ cmd_reboot,  "reboot", "Reboot the target, immediate, no arguments" }
};

#define NB_COMMANDS (uint8_t)(sizeof(cmd_table) / sizeof(cmd_table[0]))

#endif /* CMDS_H_ */
