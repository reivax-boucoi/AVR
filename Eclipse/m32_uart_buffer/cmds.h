#ifndef CMDS_H_
#define CMDS_H_

#include "periphs.h"
#include <string.h>

#ifndef NULLCHAR
#define NULLCHAR '\0'
#endif

typedef struct {
	void (*fptr_t)(void);
	const char * str;
} cmd_t;

void cmd_onLed(void);
void cmd_offLed(void);
void cmd_info(void);
void cmd_help(void);
void cmd_reboot(void);
void processCommand();

static cmd_t cmd_table[] = {
		{ cmd_onLed, "onLed" },
		{ cmd_offLed, "offLed" },
		{ cmd_info, "info" },
		{ cmd_help, "help" },
		{cmd_reboot, "reboot"}
};

static const uint8_t NB_COMMANDS = sizeof (cmd_table)/ sizeof (cmd_t);
static char cmd_buffer[50];

#endif /* CMDS_H_ */
