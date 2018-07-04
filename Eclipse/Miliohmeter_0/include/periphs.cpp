#include "periphs.h"




void periphs_init() {
	uart_init();
	TCCR1B|=(1<<CS12);
//	TCCR0A|=(1<<CS02)|(1<<CS00);
	TCCR2A|=(1<<CS22)|(1<<CS21)|(1<<CS20);
//	TIMSK0|=(1<<TOIE0);
	TIMSK1|=(1<<TOIE1);
	TIMSK2|=(1<<TOIE2);
}
