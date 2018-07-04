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
		{ cmd_display, PSTR("display")},
		{ cmd_gain, PSTR("gain")},
		{ cmd_info, PSTR("info")},
		{ cmd_help, PSTR("help")},
		{ cmd_clear, PSTR("clear")},
		{ cmd_reboot, PSTR("reboot")}
};

#define NB_COMMANDS (uint8_t)(sizeof(cmd_table) / sizeof(cmd_table[0]))

#endif /* CMDS_H_ */
