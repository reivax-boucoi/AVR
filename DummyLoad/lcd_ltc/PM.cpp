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
  //write(REG_CTRLA, 0b00001110); //calibrate on demand, continuous scan, sense only
    write(REG_CTRLA, 0b00000110); //calibrate on demand, continuous scan, all voltages
  //reading rate should be less than 10Hz (16.4ms*6)
}

bool PM::dataReady(void) {
  int abc = read(REG_ADC_STATUS, 1);
  //Serial.print("STATUS : 0b");
  //Serial.print(abc, BIN);
  return (abc && 0x8F);//IADC,GPIO2,GPIO1,SENSE2,SENSE1 ready
}


void PM::readInputPower(float *v, float *i, float *p) {
  long pl = read(REG_P1, 3);
  (*p) = pl * 0.000031875;
  long il = read(REG_I1, 2) >> 4;
  (*i) = il * 0.000770653514; // should be 0.00125
  long vl = read(REG_S1, 2) >> 4;
  (*v) = vl * 0.0255;
  
  if(il>4094 || vl>4094)Serial.println("Input measurement saturated");
}

void PM::readOutputPower(float *v, float *i, float *p) {

  long pl = read(REG_P2, 3);
  *p = pl * 0.000031875;
  long il = read(REG_I2, 2) >> 4;
  *i = il * 1.25;
  long vl = read(REG_S2, 2) >> 4;
  *v = vl * 0.0255;
  
  if(il>4094 || vl>4094)Serial.println("Output measurement saturated");
}

#define A1 0.003354016
#define B1 0.000256985

float PM::readTemp(void){//TODO Alert with register ALERT1.0 when overtemp
  long v = read(REG_G1, 2) >> 4;
  //FSR=2.048V on 12 bits (4096)
  float vsense=v*2.048/4096.0;
  
  //10k NTC (9.1K @ 22°C)
  //Resistor divider 5V-33k-Vsense-NTC-GND : vsense=NTC*5/(33k+NTC)
  //NTC(kOhm)=33*Vsense/(5-Vsense)

  float NTC=33.0*vsense/(5-vsense);
  float temp=1/(A1+B1*log(NTC/10.0))-273.15;
  return temp;
}
float PM::readBatt(void){//TODO Alert with register ALERT2.0 when under voltage
  long v = read(REG_G2, 2) >> 4;
  //FSR=2.048V on 12 bits (4096)
  //Vbatt divided by 10/(10+75)
  //total : /4096*2.048*(10+75)/10=0.00425
 /* Serial.print(v);
  Serial.print(',');
  Serial.print(v*.00425);
  Serial.print('\t');*/
  return v*.00425;
}
