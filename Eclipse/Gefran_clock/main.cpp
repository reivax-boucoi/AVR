#include <avr/io.h>
#include <util/delay.h>
#include "Clock.h"
#include "Display.h"


Display display;

int main(void){
	while(1){
		display.setNum1(1234);
		_delay_ms(500);
	}
	return 1;
}
