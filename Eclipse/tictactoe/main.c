#include <avr/io.h>
#include <avr/interrupt.h>
#include "ttt.h"
	uint8_t grid[9] = { 0,0,0,
					    0,0,0,
					    0,0,0 };
	uint8_t gameState=STOP;
int main(void){
	nextStep(grid);
	while(1){
	}
	return 1;
}
