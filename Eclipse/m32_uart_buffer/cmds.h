#ifndef CMDS_H_
#define CMDS_H_

typedef void (*fptr_t)(void);

typedef struct {
  fptr_t fptr;
  char * str;
} cmd_t;

void onLed(void);
void offLed(void);
void eor(void) ;

static cmd_t cmd_table[] = {
  { onLed, "ONLED" },
  { offLed, "OFFLED" },
  { eor, "EOR" }
};



#endif /* CMDS_H_ */
