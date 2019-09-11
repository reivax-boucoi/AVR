#include "PM.h"

void PM::write(int address, int data) {
  Wire.beginTransmission(LTC2992_addr);
  Wire.write(address); // address specifier
  Wire.write(data); // value specifier
  byte error = Wire.endTransmission();
  if (error != 0) {
    Serial.println("Error during write");
  }
  delay(1);
}

int PM::read(int address) {
  Wire.beginTransmission(LTC2992_addr);
  Wire.write(address);
  Wire.endTransmission();
  delay(1);
  Wire.requestFrom(LTC2992_addr, 1);
  while (Wire.available() == 0);
  int initial_val = Wire.read();
  return initial_val;
}

void PM::init(void) {

}
