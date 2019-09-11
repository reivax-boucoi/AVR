#include "PM.h"


void PM::write(int address, int data) {
  Wire.beginTransmission(LTC2991_add);
  Wire.write(address); // address specifier
  Wire.write(data); // value specifier
  byte error = Wire.endTransmission();
  if (error != 0) {
    Serial.println("Error during write");
  }
  delay(1);
}

int PM::read(int address) {
  Wire.beginTransmission(LTC2991_add);
  Wire.write(address);
  Wire.endTransmission();
  delay(1);
  Wire.requestFrom(LTC2991_add, 1);
  while (Wire.available() == 0);
  int initial_val = Wire.read();
  return initial_val;
}

void PM::printStatus(void) {

  Serial.print("Status HIGH: 0b");
  int status = read(0x01);
  Serial.print(status, BIN);
  Serial.print("\tStatus LOW: 0b");
  status = read(0x00);
  Serial.println(status, BIN);
}

bool PM::validDataAvailable(void) {
  return read(0x8F) & 0b00000010;
}

int PM::init(void) {

  write(0x01, 0b00111000);
  write(0x06, 0b00001001);
  // PM_write(0x08, 0b00001000);
  return 0;
}

float PM::readTinternal(void) {
  byte high = read(0x1A);
  if (high & 0x80) {
    float temp = ((high & 0x1F) << 8) + read(0x1B);
    temp = temp / 16.0;
    Serial.print("Internal Temp=");
    Serial.print(temp, 1);
    Serial.print("°C\t");
    return temp;
  } else {
    Serial.println("Internal Temp not available !");
    return -1;
  }
}

int PM::readPowerIn(void) {

  byte high = read(0x0A);
  if (high & 0x80) {
    PowerIn.pv = PowerIn.v;
    PowerIn.v = ((high & 0x3F) << 8) + read(0x0B);
    PowerIn.v = PowerIn.v * 5.0 / 16384.0 * VINATT;
  } else {
    PowerIn.valid = false;
    return -1;
  }

  high = read(0x0C);
  if (high & 0x80) {
    PowerIn.pi = PowerIn.i;
    PowerIn.i = ((high & 0x3F) << 8) + read(0x0D);
    PowerIn.i = PowerIn.i * 5.0 / 262.144 * CINMUL;
  } else {
    PowerIn.valid = false;
    return -1;
  }

  PowerIn.p = PowerIn.i * PowerIn.v;
  PowerIn.valid = true;;
  return 0;
}
int PM::readPowerOut(void) {
  byte high = read(0x0E);
  if (high & 0x80) {
    PowerOut.pv = PowerOut.v;
    PowerOut.v = ((high & 0x3F) << 8) + read(0x0F);
    PowerOut.v = PowerOut.v * 5.0 / 16384.0 * VINATT;
  } else {
    PowerOut.valid=false;
    return -1;
  }

  high = read(0x10);
  if (high & 0x80) {
    PowerOut.pi = PowerOut.i;
    PowerOut.i = ((high & 0x3F) << 8) + read(0x11);
    PowerOut.i = PowerOut.i * 5.0 / 262.144 * CINMUL;
  } else {
    PowerOut.valid=false;
    return -1;
  }

  PowerOut.p = PowerOut.i * PowerOut.v;
    PowerOut.valid=true;
  return 0;

}
void PM::display(void) {
  if(PowerIn.valid){
  Serial.print("Vin = ");
  if (abs(PowerIn.v - PowerIn.pv) < VSTAB) {
    Serial.print(PowerIn.v, 3);
  }
  Serial.print("V\tCin = ");
  if (abs(PowerIn.i - PowerIn.pi) < ISTAB) {
    Serial.print(PowerIn.i, 1);
  }
  Serial.print("mA\tPin = ");
  Serial.print(PowerIn.p, 0);

  Serial.print("mW\t");
  }
  if(PowerOut.valid){
    Serial.print("Vout = ");
  if (abs(PowerOut.v - PowerOut.pv) < VSTAB) {
    Serial.print(PowerOut.v, 3);
  }
  Serial.print("V\tCout = ");
  if (abs(PowerOut.i - PowerOut.pi) < ISTAB) {
    Serial.print(PowerOut.i, 1);
  }
  Serial.print("mA\tPout = ");
  Serial.print(PowerOut.p, 0);
  }

  if (PowerIn.p != 0 && PowerIn.valid && PowerOut.valid) {
    Serial.print("mW\tEff =");
    eff = PowerOut.p / PowerIn.p * 100.0;
    if (eff > 100) {
      Serial.println("----");
    } else {
      Serial.print(eff, 1);
      Serial.print("%\r\n");
    }
  } else {
    Serial.print("mW\r\n");
  }
}
