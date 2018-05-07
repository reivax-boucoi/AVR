#ifndef CMDS_H_
#define CMDS_H_


typedef struct {
	void (*fptr_t)(void);
	char * str;
} cmd_t;

#define MAX_PARMS 5
char *parms[MAX_PARMS];
const int N_COMMANDS = sizeof (cmd_table)/ sizeof (cmd_table);
void onLed(void);
void offLed(void);
void info(void) ;

static cmd_t cmd_table[] = {
		{ onLed, "ONLED" },
		{ offLed, "OFFLED" },
		{ info, "i" }
};



#endif /* CMDS_H_ */
