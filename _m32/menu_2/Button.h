
#ifndef Button
#define Button

#include <avr/io.h>

char ButtonPressed(unsigned char ButtonID, unsigned char portBit, unsigned char pinOfButton);
void setConfidenceLevel(unsigned char ButtonID, int level);


int PressedConfidence[nbButton];
int ReleasedConfidence[nbButton];
int pressed[nbButton];
int confidenceLevel[nbButton];

char ButtonPressed(unsigned char ButtonID, unsigned char portBit, unsigned char pinOfButton){

	if(bit_is_clear(portBit,pinOfButton)){ // if button is pressed
		PressedConfidence[ButtonID]++;
		ReleasedConfidence[ButtonID]=0;
		if(PressedConfidence[ButtonID] > confidenceLevel[ButtonID] && !pressed[ButtonID]){
			pressed[ButtonID]=1;
			PressedConfidence[ButtonID]=0;
			return 1;
		}
	}else{
		ReleasedConfidence[ButtonID]++;
		PressedConfidence[ButtonID]=0;
		if(ReleasedConfidence[ButtonID] > confidenceLevel[ButtonID] && pressed[ButtonID]){
			pressed[ButtonID]=0;
			ReleasedConfidence[ButtonID]=0;
		}
	}
	return 0;	
}


void setConfidenceLevel(unsigned char ButtonID, int level){
confidenceLevel[ButtonID]=level;
}



#endif