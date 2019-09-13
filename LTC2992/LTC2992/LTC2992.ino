
#define BLUE_LED 5
#define YELL_LED 6
#define BUTTON 2

#include <Wire.h>
#include "PM.h"
PM pm;
void setup() {
  Serial.begin(115200);
  Serial.println("\r\n\r\n\r\n\r\nStarting PM");
  Wire.begin();
  pm.init();

}


void loop() {
  if (pm.dataReady()) {
    pm.readInputPower();
    pm.readOutputPower();
    pm.calculateEff();
    Serial.print("Input : ");
    pm.displayPower(pm.input);
    Serial.print("\tOutput : ");
    pm.displayPower(pm.output);
    pm.displayEff(true);
    Serial.print("\r\n");

  } else {
    Serial.println("Data not ready");
  }

  delay(500);

}
