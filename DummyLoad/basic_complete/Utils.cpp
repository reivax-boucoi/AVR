#include "Utils.h"

int i2c_write(byte deviceAddr, int address, long data, int l) {
  Wire.beginTransmission(deviceAddr);
  Wire.write(byte(address)); // address specifier
  while (l > 0) {
    l--;
    Wire.write(byte(data >> (l * 8))); // value specifier
  }
  byte error = Wire.endTransmission();
  if (error != 0) {
    Serial.println("Error during write");
    return -1;
  }
  delay(1);
  return 0;
}

long i2c_read(byte deviceAddr, int address, int l) {
  Wire.beginTransmission(deviceAddr);
  Wire.write(byte(address));
  Wire.endTransmission(false);//repeated start !! mandatory !!

  Wire.requestFrom(deviceAddr, l);
  while (Wire.available() < l);
  long res = 0;
  for (int i = 0; i < l; i++) {
    int value = Wire.read();
    //if(address==REG_S1)Serial.println(value,HEX);
    res = (res << 8) + byte(value);
  }
  return res;
}
