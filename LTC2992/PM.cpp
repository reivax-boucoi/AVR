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
  write(REG_CTRLA, 0b00001110); //calibrate on demand, continuous scan, sense only
  write(REG_NADC, 0x80); //12 bit mode

}

bool PM::dataReady(void) {
  int status = read(REG_ADC_STATUS);
  return (status && 0x83);
}

void PM::readInputPower(void) {
  long p = read(REG_P1) + (read(REG_P1 + 1) << 8) + (read(REG_P1) << 16);
  input.p =p* 1;
  long i = read(REG_I1 + 1) + (read(REG_I1) << 4);
  input.i =i* 0.0125;
  long v = read(REG_S1 + 1) + (read(REG_S1) << 4);
  input.v *=v* 0.025;
}

void PM::displayPower(Power_T pt){
  Serial.print(pt.v,3);
  Serial.print("V\t");
  Serial.print(pt.i,3);
  Serial.print("mA\t");
  Serial.print(pt.p,3);
  Serial.print("mW\t");
}

void PM::readOutputPower(void) {

}
