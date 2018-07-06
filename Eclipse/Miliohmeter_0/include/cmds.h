#ifndef CMDS_H_
#define CMDS_H_

#include "uart_interpreter.h"
#include <avr/pgmspace.h>

typedef void (*fptr_t) (void);

void cmd_display(void);
void cmd_gain(void);
void cmd_info(void);
void cmd_help(void);
void cmd_clear(void);
void cmd_reboot(void);
void cmd_stream(void);
void cmd_hold(void);

static const fptr_t cmd_table_ptr[]={
		cmd_display,
		cmd_gain,
		cmd_info,
		cmd_help,
		cmd_clear,
		cmd_reboot,
		cmd_stream,
		cmd_hold
};

static const char* const cmd_table_str[] ={
		"display",
		"gain",
		"info",
		"help",
		"clear",
		"reboot",
		"stream",
		"hold"
};

static const char descr1[] PROGMEM = "Returns display value, sets display value with first argument";
static const char descr2[] PROGMEM = "Returns current gain, set gain or disables AGC : on/off";
static const char descr3[] PROGMEM = "Returns version number and command list";
static const char descr4[] PROGMEM = "Displays available help, use help [cmd] to get specific command help";
static const char descr5[] PROGMEM = "Clears the screen";
static const char descr6[] PROGMEM = "Reboot the target, immediate, no arguments";
static const char descr7[] PROGMEM = "Streams the display value, press ESC to kill";
static const char descr8[] PROGMEM = "Frezzes reading, call hold again to unfrezze";

static const char* const cmd_table_descr[] PROGMEM ={
		descr1,
		descr2,
		descr3,
		descr4,
		descr5,
		descr6,
		descr7,
		descr8
};

#define NB_COMMANDS (uint8_t)(sizeof(cmd_table_ptr) / sizeof(cmd_table_ptr[0]))

#endif /* CMDS_H_ */
