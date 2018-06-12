#include <avr/io.h>

#ifndef TTT_H_
#define TTT_H_

#define SINGLE 0
#define DUAL 1

#define STOPPED 0
#define PLAYING 1
#define PLAYERWON 2
#define COMPUTERWON 3
#define DRAW 9

#define COMPUTER 2
#define EMPTY 0
#define PLAYER 1

class Ttt {
public:
	Ttt();
	~Ttt();
	//void setMode(bool mode);
	uint8_t play(void); // 9 if draw
	uint8_t playerInput(uint8_t slot); // 0 : valid input, Invalid : 2, won : 1
	uint8_t getSlotState(uint8_t slot);
	uint8_t getGameState(void);

private:
	uint8_t grid[9];
	uint8_t gameState = STOPPED;
	uint8_t checkWon(uint8_t who);
};

#endif /* TTT_H_ */
