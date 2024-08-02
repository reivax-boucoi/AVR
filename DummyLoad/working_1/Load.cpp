#include "Load.h"



void Load::init(void) {
  pinMode(LEDR2_PIN, OUTPUT);
  pinMode(LEDB2_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, 0);
  digitalWrite(LEDR2_PIN, 0);
  digitalWrite(LEDB2_PIN, 0);
  onState = false;
  setCurrent = 0.0;
  actualDACVal = 0;

  i2c_write(DAC_ADDR, VREF_REG << 3, 0x0001, 2);
  i2c_write(DAC_ADDR, GAIN_REG << 3, 1 << 8, 2);
  i2c_write(DAC_ADDR, PD_REG << 3, 0x0001, 2);

}

void Load::regulate(float current, float temp) {

  if (temp >= TEMP_MAX && onState) {
    Serial.println("Temp hot exceeded !");
    fault = true;
  }
  
  if (onState && !fan_on && temp >= (fan_temp+FAN_HYST)) {
    digitalWrite(FAN_PIN, HIGH);
    fan_on = true;
    Serial.println("Turning fan on !");
  }
  if (fan_on && temp < (fan_temp-FAN_HYST)) {
    digitalWrite(FAN_PIN, LOW);
    fan_on = false;
    Serial.println("Turning fan off !");
  }
  
  if (fault) {
    Serial.println("Fault detected, load shutting down");
    off();
  }

  digitalWrite(LEDR2_PIN, fault);
}

void Load::on(float current) {
  onState = true;
  fault = false;
  setCurrent = current;
  actualDACVal = current * DAC_CAL_COEFF;
  DAC_set();
  DAC_on();
  Serial.print("DAC set to ");
  Serial.print(current);
  Serial.print("A with code ");
  Serial.println(actualDACVal);
  digitalWrite(LEDB2_PIN, HIGH);
}

void Load::set(float current) {
  setCurrent = current;
  actualDACVal = current * DAC_CAL_COEFF;
  DAC_set();
}

void Load::off(void) {
  DAC_set(0);
  DAC_off();
  onState = false;
  Serial.println("Dac off !");
  digitalWrite(LEDB2_PIN, LOW);
}

void Load::DAC_set(void) {
  if (actualDACVal > 4095) {
    Serial.println("Dac saturating up !");
    actualDACVal = 4095;
  } else if (actualDACVal < 0) {
    Serial.println("Dac saturating down !");
    actualDACVal = 0;
  }
  DAC_set(actualDACVal);

}

void Load::DAC_set(int val) {
  i2c_write(DAC_ADDR, DAC_REG, val, 2);
}

void Load::DAC_on(void) {
  i2c_write(DAC_ADDR, PD_REG << 3, 0x0000, 2);
}

void Load::DAC_off(void) {
  i2c_write(DAC_ADDR, PD_REG << 3, 0x0001, 2);
}
