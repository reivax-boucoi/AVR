#include "PM.h"

int PM::write(int address, int data) {
  Wire.beginTransmission(LTC2992_addr);
  Wire.write(byte(address)); // address specifier
  Wire.write(byte(data)); // value specifier
  byte error = Wire.endTransmission();
  if (error != 0) {
    Serial.println("Error during write");
    return -1;
  }
  delay(1);
  return 0;
}

long PM::read(int address, int l) {
  Wire.beginTransmission(LTC2992_addr);
  Wire.write(byte(address));
  Wire.endTransmission(false);//repeated start !! mandatory !!

  Wire.requestFrom(LTC2992_addr, l);
  while (Wire.available() < l);
  long res = 0;
  for (int i = 0; i < l; i++) {
    int value = Wire.read();
    //if(address==REG_S1)Serial.println(value,HEX);
    res = (res << 8) + byte(value);
  }
  return res;
}

void PM::init(void) {
  write(REG_CTRLA, 0b00001110); //calibrate on demand, continuous scan, sense only
}

bool PM::dataReady(void) {
  int abc = read(REG_ADC_STATUS, 1);
  //Serial.print("STATUS : 0b");
  //Serial.print(abc, BIN);
  return (abc && 0x83);
}

void PM::readInputPower(void) {
  long p = read(REG_P1, 3);
  input.p = p * 0.031875;
  long i = read(REG_I1, 2) >> 4;
  input.i = i * 1.25;
  long v = read(REG_S1, 2) >> 4;
  input.v = v * 0.0255;
  setRanges(&input);
}
void PM::setRanges(Power_T *pt) {
  if (pt->i > 999.0) {
    pt->amps = true;
  }
  if (pt->i < 900.0) {
    pt->amps = false;
  }
  if (pt->p > 999.0) {
    pt->watts = true;
  }
  if (pt->p < 900.0) {
    pt->watts = false;
  }

}
void PM::displayPower(Power_T pt, outFormat f) {
  if (f == CSV) {
    Serial.print(pt.v, 1);
    Serial.write(',');
    Serial.print(pt.i, 0);
    Serial.write(',');
    Serial.print(pt.p, 0);

  } else if (f == HRF) {
    Serial.print(pt.v, 1);
    Serial.print("V\t");
    if (pt.amps) {
      Serial.print(pt.i / 1000.0, 2);
      Serial.print("A\t");
    } else {
      Serial.print(pt.i, 0);
      Serial.print("mA\t");
    }
    if (pt.watts) {
      Serial.print(pt.p / 1000.0, 2);
      Serial.print("W\t\t");
    } else {
      Serial.print(pt.p, 0);
      Serial.print("mW\t\t");
    }
  }
}

void PM::readOutputPower(void) {

  long p = read(REG_P2, 3);
  output.p = p * 0.031875;
  long i = read(REG_I2, 2) >> 4;
  output.i = i * 1.25;
  long v = read(REG_S2, 2) >> 4;
  output.v = v * 0.0255;
  setRanges(&output);
}


void PM::calculateEff(void) {
  if (input.p != 0.0 && output.p != 0.0) {
    float val = 100.0 * output.p / input.p;
    if (val < 100.0 && val >= 5.0) {
      eff = val;
      ploss = input.p - output.p;
      if (ploss > 999.0) {
        watts_ploss = true;
      }
      if (ploss < 900.0) {
        watts_ploss = false;
      }
    } else {
      eff = -1;
    }
  }
}
void PM::displayEff(bool all, outFormat f) {
  if (f == CSV) {
    Serial.write(',');
    Serial.print(eff, 1);
    if (all) {
      Serial.write(',');
      Serial.print(ploss, 0);
    }
  } else if (f == HRF) {
    if (eff > 0.0) {
      Serial.print("\tEfficiency : ");
      Serial.print(eff, 1);
      Serial.print("%\t");
      if (all) {
        Serial.print("P_loss : ");
        if (watts_ploss) {
          Serial.print(ploss / 1000.0, 2);
          Serial.print("W\t");
        } else {
          Serial.print(ploss, 0);
          Serial.print("mW\t");
        }
      }
    }
  }
}
