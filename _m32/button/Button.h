// CAUTION ! : For use with one button only !

#ifndef Button
#define Button

#include <avr/io.h>

int PressedConfidence=0;
int ReleasedConfidence=0;
int pressed=0;
int confidenceLevel=100;

char ButtonPressed(unsigned char portBit, unsigned char pinOfButton){

	if(bit_is_clear(portBit,pinOfButton)){ // if button is pressed
		PressedConfidence++;
		ReleasedConfidence=0;
		if(PressedConfidence > confidenceLevel && !pressed){
			pressed=1;
			PressedConfidence=0;
			return 1;
		}
	}else{
		ReleasedConfidence++;
		PressedConfidence=0;
		if(ReleasedConfidence > confidenceLevel && pressed){
			pressed=0;
			ReleasedConfidence=0;
		}
	}
	return 0;	
	

}

void setConfidenceLevel(int level){
confidenceLevel=level;
}



#endif