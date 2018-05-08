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

void onLed(void);
void offLed(void);
void info(void);
void help(void);
void reboot(void);
void processCommand();

static cmd_t cmd_table[] = {
		{ onLed, "onLed" },
		{ offLed, "offLed" },
		{ info, "info" },
		{ help, "help" },
		{reboot, "reboot"}
};

static const uint8_t NB_COMMANDS = sizeof (cmd_table)/ sizeof (cmd_t);
static char cmd_buffer[50];

#endif /* CMDS_H_ */
