#include "Load.h"




void Load::init(void) {
  onState = false;
  setCurrent = 0.0;
  actualDACVal = 0;

  
}

void Load::regulate(float current) {
  if(onState && abs(current-setCurrent)>LOAD_REG_THRESHOLD){
    Serial.println("Regulating");
    if((current-setCurrent)<0){
      actualDACVal++;
    }else{
      actualDACVal--;
    }
    DAC_set();
  }
}

void Load::on(float current) {
  onState=true;
  setCurrent=current;
  actualDACVal=current*DAC_CAL_COEFF;
  Serial.print("DAC set to ");
  Serial.print(current);
  Serial.print("with code ");
  Serial.println(actualDACVal);
  DAC_set();
  DAC_on();
}

void Load::off(void) {
  DAC_set(0);
  DAC_off();
  onState=false;
}

void Load::DAC_set(void) {
  if(actualDACVal>4095){
    Serial.println("Dac saturating up !");
    actualDACVal=4095;
  }else if(actualDACVal<0){
    Serial.println("Dac saturating down !");
    actualDACVal=0;    
  }
  DAC_set(actualDACVal);
  
}

void Load::DAC_set(int val){
  
  //i2c stuff here
  
}

void Load::DAC_on(void) {

  //i2c stuff here
}

void Load::DAC_off(void) {

  //i2c stuff here
}
