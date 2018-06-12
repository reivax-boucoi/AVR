
#include "Ttt.h"

Ttt::Ttt() {

}

Ttt::~Ttt() {
}

uint8_t Ttt::play(void) {
	for(uint8_t i=0;i<9;i++){
		if(this->grid[i]==EMPTY){
			this->grid[i]=COMPUTER;
			if(checkWon(COMPUTER)){
				return i;
			}
			this->grid[i]=EMPTY;
		}
	}
	return 9;
}

uint8_t Ttt::playerInput(uint8_t slot) {
	if(this->grid[slot])return 2;
	this->grid[slot]=PLAYER;
	return checkWon(PLAYER);

}

uint8_t Ttt::getSlotState(uint8_t slot) {
	return this->grid[slot];
}

uint8_t Ttt::checkWon(uint8_t who){

	for(uint8_t i=0;i<3;i++){
		if(this->grid[i*3]==who && this->grid[(i*3)+1]==who && this->grid[(i*3)+1]==who )return 1; // row
		if(this->grid[i]==who && this->grid[i+3]==who && this->grid[i+6]==who )return 1; // col
	}

	if(this->grid[0]==who && this->grid[4]==who && this->grid[8]==who )return 1; // diag right
	if(this->grid[2]==who && this->grid[4]==who && this->grid[6]==who )return 1; // diag left
	return 0;
}

uint8_t Ttt::getGameState(void){
	return gameState;
}
