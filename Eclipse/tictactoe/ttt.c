#include "ttt.h"
uint8_t checkWin(uint8_t* grid){

	uint8_t value = 0;
	uint8_t count = 0;
	for(uint8_t i=0; i<9; i++){
		if(grid[i]==0) {count++;}
	}

	if(grid[0]+grid[1]+grid[2]==3 || grid[3]+grid[4]+grid[5]==3 || grid[6]+grid[7]+grid[8]==3 ||
		grid[0]+grid[3]+grid[6]==3 || grid[1]+grid[4]+grid[7]==3 || grid[2]+grid[5]+grid[8]==3 ||
		grid[0]+grid[4]+grid[8]==3 || grid[2]+grid[4]+grid[6]==3){
			value = 3; //COMPUTER win
	}
	else if(grid[0]+grid[1]+grid[2]==12 || grid[3]+grid[4]+grid[5]==12 || grid[6]+grid[7]+grid[8]==12 ||
			grid[0]+grid[3]+grid[6]==12 || grid[1]+grid[4]+grid[7]==12 || grid[2]+grid[5]+grid[8]==12 ||
			grid[0]+grid[4]+grid[8]==12 || grid[2]+grid[4]+grid[6]==12){
			value = 2; //PLAYER win
	}
	else if(count==0) value = DRAW;
	else  value = PLAYING; //continue

	return value;
}

uint8_t nextStep(uint8_t* grid){
	uint8_t cont = 9;
	uint8_t temp[9];

	//1 move computer win
	for(uint8_t i=0; i<9; i++){
		if(grid[i]==0 && cont==9){
			for(uint8_t j=0; j<9;j++){temp[j]=grid[j];}
			temp[i]=1;
			if( checkWin(temp)==3)cont = i;
		}
	}

	//1 move player win
	for(uint8_t i = 0; i<9; i++){
		if(grid[i]==0 && cont==9){
			for(uint8_t j = 0; j<9;j++){temp[j]=grid[j];}
			temp[i]=4;
			if( (checkWin(temp))==2) cont = i;
		}
	}
	if(cont>8) {cont = twoMove(grid);}//two move win/loss

	//random
	if(cont>8){
		for(uint8_t i=0; i<9; i++){
			if(grid[i]==0 && cont==9)cont = i;
		}
	}
	return cont;
}

uint8_t twoMove(uint8_t* grid){
	uint8_t cont = 9;
	if(grid[4]==0 && cont==9){
		if(grid[1]==0 && grid[3]==0 && grid[5]==0 && grid[7]==0){
			if((grid[0]==0 && grid[2]==0 && grid[6]==0 && grid[8]==4)||
			(grid[0]==0 && grid[2]==0 && grid[6]==4 && grid[8]==0)||
			(grid[0]==4 && grid[2]==0 && grid[6]==0 && grid[8]==0)||
			(grid[0]==0 && grid[2]==4 && grid[6]==0 && grid[8]==0)) {
				cont=4;
			}else if((grid[0]==1 && grid[2]==0 && grid[6]==0 && grid[8]==4)||
			(grid[0]==4 && grid[2]==0 && grid[6]==0 && grid[8]==1)) {
				cont=2;
			}else if((grid[0]==0 && grid[2]==4 && grid[6]==1 && grid[8]==0)||
			(grid[0]==0 && grid[2]==1 && grid[6]==4 && grid[8]==0)) {
				cont=0;
			}
		}
		if(grid[1]==4 && grid[3]==0 && grid[5]==0 && grid[7]==0){
			if(grid[0]+grid[2]==5){
				if(grid[6]==0 && grid[8]==4){
					cont=6;
				}else if(grid[6]==4 && grid[8]==0){
					cont=8;
				}
			}
		}else if(grid[1]==0 && grid[3]==4 && grid[5]==0 && grid[7]==0){
			if(grid[0]+grid[6]==5){
				if(grid[2]==0 && grid[8]==4){
					cont=2;
				}else if(grid[2]==4 && grid[8]==0){
					cont=8;
				}
			}
		}else if(grid[1]==0 && grid[3]==0 && grid[5]==4 && grid[7]==0){
			if(grid[8]+grid[2]==5){
				if(grid[0]==0 && grid[6]==4){
					cont=0;
				}else if(grid[0]==4 && grid[6]==0){
					cont=6;
				}
			}
		}else if(grid[1]==0 && grid[3]==0 && grid[5]==0 && grid[7]==4){
			if(grid[8]+grid[6]==5){
				if(grid[0]==0 && grid[2]==4){
					cont=0;
				}else if(grid[0]==4 && grid[2]==0){
					cont=2;
				}
			}
		}
	}
	else if(grid[4]==1 && cont==9){
		if(grid[0]==0 && grid[1]==0 && grid[2]==4 &&
			grid[3]==0 && grid[5]==0 &&
			grid[6]==4 && grid[7]==0 && grid[8]==0) {
			cont = 1;
		}else if(grid[0]==4 && grid[1]==0 && grid[2]==0 &&
			grid[3]==0 && grid[5]==0 &&
			grid[6]==0 && grid[7]==0 && grid[8]==4) {
			cont = 3;
		}else if(grid[0]==0 && grid[1]==4 && grid[2]==0 &&
			grid[3]==4 &&
			grid[6]==0) {
			cont = 0;
		}else if(grid[6]==0 && grid[3]==4 && grid[0]==0 &&
			grid[7]==4 &&
			grid[8]==0) {
			cont = 6;
		}else if(grid[8]==0 && grid[7]==4 && grid[6]==0 &&
			grid[5]==4 &&
			grid[2]==0) {
			cont = 8;
		}else if(grid[2]==0 && grid[5]==4 && grid[8]==0 &&
			grid[4]==4 &&
			grid[0]==0) {
			cont = 2;
		}else if(grid[0]==0 && grid[1]==0 && grid[2]==4 &&
			grid[3]==4){
			if(grid[6]!=1) {
				cont = 0;
			}
		}
		else if(grid[6]==0 && grid[3]==0 && grid[0]==4 &&
			grid[7]==4){
			if(grid[8]!=1) {
				cont = 6;
			}
		}else if(grid[8]==0 && grid[7]==0 && grid[6]==4 &&
			grid[5]==4){
			if(grid[2]!=1) {
				cont = 8;
			}
		}else if(grid[2]==0 && grid[5]==0 && grid[8]==4 &&
			grid[1]==4){
			if(grid[0]!=1) {
				cont = 2;
			}
		}
	}else if(grid[4]==4 && cont==9){
		if(grid[1]==0 && grid[3]==0 && grid[5]==0 && grid[7]==0){
			if(grid[0]==0 && grid[2]==0 && grid[6]==0 && grid[8]==1) cont=0;
			if(grid[0]==0 && grid[2]==0 && grid[6]==1 && grid[8]==0) cont=2;
			if(grid[0]==0 && grid[2]==1 && grid[6]==0 && grid[8]==0) cont=6;
			if(grid[0]==1 && grid[2]==0 && grid[6]==0 && grid[8]==0) cont=8;
		}else if(grid[1]==0 && grid[7]==0) {
			cont = 1;
		}else if(grid[3]==0 && grid[5]==0) {
			cont = 3;
		}else if(grid[0]==0 && grid[8]==0) {
			cont = 0;
		}else if(grid[2]==0 && grid[6]==0) {
			cont = 2;
		}
	}
	return cont;
}
