#ifndef CMDS_H_
#define CMDS_H_


typedef struct {
	void (*fptr_t)(void);
	char * str;
} cmd_t;

static cmd_t cmd_table[] = {
		{ onLed, "ONLED" },
		{ offLed, "OFFLED" },
		{ info, "i" }
};

const int N_COMMANDS = sizeof (cmd_table)/ sizeof (cmd_table);

static	int		last_address = 0;
static	int		cmd_buffer_index = 0;
static	char	cmd_buffer[50];
static	char	last_cmd[50];

void onLed(void);
void offLed(void);
void info(void) ;





#endif /* CMDS_H_ */
