
#define BLUE_LED 5
#define YELL_LED 6
#define BUTTON 2


#include "PM.h"
PM pm;
void setup() {
  Serial.begin(115200);
  Serial.println("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\nStarting PM");
  pm.init();

}


void loop() {

  if (pm.dataReady()) {
    pm.readInputPower();
    pm.readOutputPower();
    Serial.print("\tInput : ");
    pm.displayPower(pm.input);
    Serial.print("\r\n");
  } else {
    Serial.println("Data not ready");
  }
  delay(2000);

}
