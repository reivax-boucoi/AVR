#ifndef TTT_H
#define TTT_H

#include <avr/io.h>

#define STOP
#define DRAW 0
#define PLAYING 1
#define COMPUTER 3
#define PLAYER 2
	//1: computer, 4: player, 0: empty
	//2 | 1 | 0
	//5 | 4 | 3
	//8 | 7 | 6

uint8_t checkWin(uint8_t* grid);
uint8_t nextStep(uint8_t* grid);
uint8_t  twoMove(uint8_t* grid);



#endif //TTT_H
