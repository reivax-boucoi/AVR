#include "PM.h"
#include <Wire.h>

PM myPM;




void setup() {
  Wire.begin();
  Serial.begin(115200);
}


void loop() {
  byte error;
  Wire.beginTransmission(LTC2991_add);
  error = Wire.endTransmission();

  if (error == 0) {
    myPM.init();
    //printStatus();
    //readTinternal();
    int res=myPM.readPowerIn();
    
    res+=myPM.readPowerOut();
    if(res==0){
      myPM.display();
    }else{
      Serial.println("something went wrong");
    }
  } else if (error == 4) {
    Serial.print("Unknown error\r\n");
  }else{
    Serial.println("Error !");
  }
  delay(100);
}
