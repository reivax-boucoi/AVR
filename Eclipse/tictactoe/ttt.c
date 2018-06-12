
int8_t checkWin(uint8_t* grid);
int8_t nextStep(uint8_t* grid);
int8_t twoMove(uint8_t* grid);


int8_t checkWin(uint8_t* grid){
	//grid = 	2 | 1 | 0
	//	5 | 4 | 3
	//    	8 | 7 | 6	cell locations for grid

	//grid[i] = 1: computer, grid[i] = 4: human, grid[i] = 0: empty cell
	int8_t value = 0;
	int count = 0;
	int i;

	//count the number of unassigned cells
	for(i = 0; i<9; i++){
		if(grid[i]==0) {count++;}
	}

	//checks for winning conditions (comp)
	if(grid[0]+grid[1]+grid[2]==3 || grid[3]+grid[4]+grid[5]==3 || grid[6]+grid[7]+grid[8]==3 || //horizontal check
		grid[0]+grid[3]+grid[6]==3 || grid[1]+grid[4]+grid[7]==3 || grid[2]+grid[5]+grid[8]==3 || //vertical check
		grid[0]+grid[4]+grid[8]==3 || grid[2]+grid[4]+grid[6]==3){ //diagnal check
			value = 3; //computer wins
	}
	//checks for winning conditions (human)
	else if(grid[0]+grid[1]+grid[2]==12 || grid[3]+grid[4]+grid[5]==12 || grid[6]+grid[7]+grid[8]==12 ||//horontal check
			grid[0]+grid[3]+grid[6]==12 || grid[1]+grid[4]+grid[7]==12 || grid[2]+grid[5]+grid[8]==12 || //vertical check
			grid[0]+grid[4]+grid[8]==12 || grid[2]+grid[4]+grid[6]==12){ //diagnal check
			value = 2; //human wins
	}
	//else game goes on if there are spots left;
	else if(count==0) value = 0; //tied game
	else  value = 1; //game keeps going

	return value;
}

int8_t nextStep(uint8_t* grid){
	//grid = 	2 | 1 | 0
	//	5 | 4 | 3
	//    	8 | 7 | 6	cell locations for grid

	//assume there's at least one spot to write into
	//grid[i] = 1: computer, grid[i] = 4: human, grid[i] = 0: empty cell
	//initialize step variable cont
	int8_t cont = 9;
	uint8_t temp[9];
	int i, j;

	//check for 1 move computer win
	for(i = 0; i<9; i++){
		if(grid[i]==0 && cont==9){
			for(j = 0; j<9;j++){temp[j]=grid[j];} //create a temporary array
			temp[i]=1;//check if the next step would win
			if( checkWin(temp)==3){cont = (int8_t)i;}
		}
	}

	//else check for 1 move human win
	for(i = 0; i<9; i++){
		if(grid[i]==0 && cont==9){
			for(j = 0; j<9;j++){temp[j]=grid[j];} //create a temporary array
			temp[i]=4;//check if human would win if don't block
			if( (checkWin(temp))==2) {cont = (int8_t)i;}
		}
	}
	if(cont>8) {cont = twoMove(grid);}//check for two move win/loss
	//pick the first available cell if there's no two move wins
	if(cont>8){
		for(i=0; i<9; i++){
			if(grid[i]==0 && cont==9){cont = (int8_t)i;}
		}
	}
	return cont;
}

int8_t twoMove(uint8_t* grid){
	//grid = 	2 | 1 | 0
	//	5 | 4 | 3
	//    	8 | 7 | 6	cell locations for grid

	//assume there's at least one spot to write into
	//grid[i] = 1: computer, grid[i] = 4: human, grid[i] = 0: empty cell
	//initialize step variable cont
	int8_t cont = 9;

	//case with no one in middle
	if(grid[4]==0 && cont==9){
		if(grid[1]==0 && grid[3]==0 && grid[5]==0 && grid[7]==0){
			//case 8 x4 orientations
			if((grid[0]==0 && grid[2]==0 && grid[6]==0 && grid[8]==4)||
			(grid[0]==0 && grid[2]==0 && grid[6]==4 && grid[8]==0)||
			(grid[0]==4 && grid[2]==0 && grid[6]==0 && grid[8]==0)||
			(grid[0]==0 && grid[2]==4 && grid[6]==0 && grid[8]==0)) {cont=4;}
			//case 9 x4 orientations
			else if((grid[0]==1 && grid[2]==0 && grid[6]==0 && grid[8]==4)||
			(grid[0]==4 && grid[2]==0 && grid[6]==0 && grid[8]==1)) {cont=2;}
			else if((grid[0]==0 && grid[2]==4 && grid[6]==1 && grid[8]==0)||
			(grid[0]==0 && grid[2]==1 && grid[6]==4 && grid[8]==0)) {cont=0;}
		}
		//case 10 x8 orientations
		if(grid[1]==4 && grid[3]==0 && grid[5]==0 && grid[7]==0){
			if(grid[0]+grid[2]==5){
				if(grid[6]==0 && grid[8]==4){cont=6;}
				else if(grid[6]==4 && grid[8]==0){cont=8;}
			}
		}
		else if(grid[1]==0 && grid[3]==4 && grid[5]==0 && grid[7]==0){
			if(grid[0]+grid[6]==5){
				if(grid[2]==0 && grid[8]==4){cont=2;}
				else if(grid[2]==4 && grid[8]==0){cont=8;}
			}
		}
		else if(grid[1]==0 && grid[3]==0 && grid[5]==4 && grid[7]==0){
			if(grid[8]+grid[2]==5){
				if(grid[0]==0 && grid[6]==4){cont=0;}
				else if(grid[0]==4 && grid[6]==0){cont=6;}
			}
		}
		else if(grid[1]==0 && grid[3]==0 && grid[5]==0 && grid[7]==4){
			if(grid[8]+grid[6]==5){
				if(grid[0]==0 && grid[2]==4){cont=0;}
				else if(grid[0]==4 && grid[2]==0){cont=2;}
			}
		}
	}
	//case with computer in middle
	else if(grid[4]==1 && cont==9){
		//case 1 x2 orientations
		if(grid[0]==0 && grid[1]==0 && grid[2]==4 &&
			grid[3]==0 && grid[5]==0 &&
			grid[6]==4 && grid[7]==0 && grid[8]==0) {cont = 1;}
		else if(grid[0]==4 && grid[1]==0 && grid[2]==0 &&
			grid[3]==0 && grid[5]==0 &&
			grid[6]==0 && grid[7]==0 && grid[8]==4) {cont = 3;}
		//case 2 x4 orientations
		else if(grid[0]==0 && grid[1]==4 && grid[2]==0 &&
			grid[3]==4 &&
			grid[6]==0) {cont = 0;}
		else if(grid[6]==0 && grid[3]==4 && grid[0]==0 &&
			grid[7]==4 &&
			grid[8]==0) {cont = 6;}
		else if(grid[8]==0 && grid[7]==4 && grid[6]==0 &&
			grid[5]==4 &&
			grid[2]==0) {cont = 8;}
		else if(grid[2]==0 && grid[5]==4 && grid[8]==0 &&
			grid[4]==4 &&
			grid[0]==0) {cont = 2;}
		//case 3+4 x4 orientations
		else if(grid[0]==0 && grid[1]==0 && grid[2]==4 &&
			grid[3]==4){
			if(grid[6]!=1) {cont = 0;}
		}
		else if(grid[6]==0 && grid[3]==0 && grid[0]==4 &&
			grid[7]==4){
			if(grid[8]!=1) {cont = 6;}
		}
		else if(grid[8]==0 && grid[7]==0 && grid[6]==4 &&
			grid[5]==4){
			if(grid[2]!=1) {cont = 8;}
		}
		else if(grid[2]==0 && grid[5]==0 && grid[8]==4 &&
			grid[1]==4){
			if(grid[0]!=1) {cont = 2;}
		}
	}
	//cases with human in middle
	else if(grid[4]==4 && cont==9){
		//case 5 x4 orientations
		if(grid[1]==0 && grid[3]==0 && grid[5]==0 && grid[7]==0){
			if(grid[0]==0 && grid[2]==0 && grid[6]==0 && grid[8]==1) {cont=0;}
			if(grid[0]==0 && grid[2]==0 && grid[6]==1 && grid[8]==0) {cont=2;}
			if(grid[0]==0 && grid[2]==1 && grid[6]==0 && grid[8]==0) {cont=6;}
			if(grid[0]==1 && grid[2]==0 && grid[6]==0 && grid[8]==0) {cont=8;}
		}
		//case 6 x2 orientations
		else if(grid[1]==0 && grid[7]==0) {cont = 1;}
		else if(grid[3]==0 && grid[5]==0) {cont = 3;}
		//case 7 x2 orientations
		else if(grid[0]==0 && grid[8]==0) {cont = 0;}
		else if(grid[2]==0 && grid[6]==0) {cont = 2;}
	}
	return cont;
}
