#ifndef CMDS_H_
#define CMDS_H_

#include "periphs.h"
#include <string.h>

#ifndef NULLCHAR
#define NULLCHAR '\0'
#endif

typedef struct {
	void (*fptr_t)(void);
	char * str;
} cmd_t;

void onLed(void);
void offLed(void);
void info(void);
void ProcessCommand();

static cmd_t cmd_table[] = {
		{ onLed, "ONLED" },
		{ offLed, "OFFLED" },
		{ info, "info" }
};

const int N_COMMANDS = sizeof (cmd_table)/ sizeof (cmd_t);
static	char	cmd_buffer[50];

#endif /* CMDS_H_ */
