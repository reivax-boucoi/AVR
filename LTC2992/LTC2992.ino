
#define BLUE_LED 5
#define YELL_LED 6
#define BUTTON 2


#include <Wire.h>
#include "PM.h"
PM pm;
void setup(){
  Serial.begin(115200);
  Serial.println("Starting PM");
  Wire.begin();
  pm.init();
  
}


void loop(){

  byte error;
  Wire.beginTransmission(LTC2992_addr);
  error = Wire.endTransmission();

  if (error == 0) {
    Serial.println("Comm started !");
    if(pm.dataReady()){
      pm.readInputPower();
      pm.readOutputPower();
      Serial.print("Input : ");
      pm.displayPower(pm.input);
      Serial.print("\r\n");
    }else{
      Serial.println("Data not ready");
    }
  }else{
    Serial.println("Couldn't talk to LTC2992 !");
  }
  delay(1000);
  
}