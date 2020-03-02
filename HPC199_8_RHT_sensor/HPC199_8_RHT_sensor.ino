#include "Wire.h"

#define HPC_addr 0x40

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Hi there !");
}
float getTemperature() {
  Wire.beginTransmission(HPC_addr);
  Wire.write(byte(0xF3));
  Wire.endTransmission();
  delay(75);
  Wire.requestFrom(HPC_addr, 3);
  if (3 <= Wire.available()) {
    uint8_t msb = Wire.read();
    uint8_t lsb = Wire.read();
    uint8_t chk = Wire.read();
    return (((((uint16_t) msb << 8) | (uint16_t) lsb) & 0xFFFC) * (175.72 / 65536.0)) - 46.85;
  }
  return -1;

}
float getHumidity() {
  Wire.beginTransmission(HPC_addr);
  Wire.write(byte(0xF5));//humidity
  Wire.endTransmission();
  delay(75);
  Wire.requestFrom(HPC_addr, 3);
  if (3 <= Wire.available()) {
    uint8_t msb = Wire.read();
    uint8_t lsb = Wire.read();
    uint8_t chk = Wire.read();
    return ((( ((uint16_t) msb << 8) | (uint16_t) lsb) & 0xFFFC) * (125.0 / 65536.0)) - 6.0;
  }
  return -1;
}
void loop() {

  Serial.print("Temperature : ");
  Serial.print(getTemperature(), 1);
  Serial.print("\tHumidity : ");
  Serial.println(getHumidity(), 1);
  delay(2000);

}
