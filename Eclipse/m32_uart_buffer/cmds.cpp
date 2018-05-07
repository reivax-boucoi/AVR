#include "cmds.h"
#include <avr/io.h>


void onLed(void) {
	PORTB|=(1<<PB0);
}

void offLed(void) {
	PORTB&=~(1<<PB0);
}

void eor(void) {
}
