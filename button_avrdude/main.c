
//#define F_CPU 16000000
#define F_CPU 1000000

#include <avr/io.h>
int checkButton();
int toggleButton();
  int state=1;
  int released=0;
  int pressedCertainty=0;
  int releasedCertainty=0;
  int boolean=0;

int main(void){

  DDRB |= 1<< PINB1 ;
  DDRB &=~(1<<PINB0);

  while(1){
	
	
	//if(checkButton()){
		
	//}
  }
  return 0;
}

int checkButton(){

 if(bit_is_clear(PINB,0)){
	releasedCertainty=0;
	pressedCertainty++;	
	 if(released && pressedCertainty>500){
		 pressedCertainty=0;
		 released=0;
		return 1;
	}
 }else{
	pressedCertainty=0;
	releasedCertainty++;
	if(releasedCertainty>500){
		 released=1;
		 releasedCertainty=0;
		return 0;
	 }
 }
}

int toggleButton(){
	if(checkButton() && state==0){
		
	}
}