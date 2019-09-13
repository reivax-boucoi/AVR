#include "PM.h"


    int PM::write(int address, int data){
  /* Send Start */
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWEA);
  while (!(TWCR & _BV(TWINT)));
  if ((TWSR & 0xF8) != TW_START) {
    return -1;
  }

  /* Send SLA+W */
  TWDR = (LTC2992_addr << 1) | TW_WRITE; // SLA+W
  TWCR = (1 << TWINT) | (1 << TWEN); // Start transmission
  while (!(TWCR & _BV(TWINT)));
  if ((TWSR & 0xF8) != TW_MT_SLA_ACK) {
    return -1;
  }

  TWDR = address; // Send address of register
  TWCR = (1 << TWINT) | (1 << TWEN); // Start transmission
  while (!(TWCR & _BV(TWINT)));
  if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
    return -1;
  }
  TWDR = data; // Set value of register
  TWCR = (1 << TWINT) | (1 << TWEN); // Start transmission
  while (!(TWCR & _BV(TWINT)));
  if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
    return -1;
  }

  /* Send Stop condition */
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
  _delay_ms(1);


}
long PM::read(int address, int l) {

  /* Send Start */
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWEA);
  while (!(TWCR & _BV(TWINT)));
  if ((TWSR & 0xF8) != TW_START) {
    return -1;
  }

  /* Send SLA+W */
  TWDR = (LTC2992_addr << 1) | TW_WRITE; // SLA+W
  TWCR = (1 << TWINT) | (1 << TWEN); // Start transmission
  while (!(TWCR & _BV(TWINT)));
  if ((TWSR & 0xF8) != TW_MT_SLA_ACK) {
    return -1;
  }

  /* Set Address to read */
  TWDR = address; //address to read at
  TWCR = (1 << TWINT) | (1 << TWEN); // Start transmission
  while (!(TWCR & _BV(TWINT)));
  if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
    return -1;
  }

  /* Send Stop condition */
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
  _delay_ms(10);
  /* Send Start */
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWEA);
  while (!(TWCR & _BV(TWINT)));
  if ((TWSR & 0xF8) != TW_START) {
    return -1;
  }
  _delay_ms(1);
  
  /* Send Start */
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWEA);
  while (!(TWCR & _BV(TWINT)));
  if ((TWSR & 0xF8) != TW_START) {
    return -1;
  }

  /* Send SLA+R */
  TWDR = (LTC2992_addr << 1) | TW_READ; // SLA+R
  TWCR = (1 << TWINT) | (1 << TWEN); // Start read
  while (!(TWCR & _BV(TWINT)));
  if ((TWSR & 0xF8) != TW_MR_SLA_ACK) {
    return -1;
  }

  /* Read Register(s) */
  long res = 0;
  TWCR = (1 << TWINT) | (1 << TWEN) | _BV(TWEA); // Start Read, ACK enabled
  while (!(TWCR & _BV(TWINT)));
  if ((TWSR & 0xF8) != TW_MR_DATA_ACK) {
    return -1;
  }
  res = TWDR;
  while (l > 1) {
    l--;
    res = res << 8;
    TWCR = (1 << TWINT) | (1 << TWEN); // no TWEA--> send NACK after last byte
    while (!(TWCR & _BV(TWINT)));
    if ((TWSR & 0xF8) != TW_MR_DATA_NACK) {
      return -1;
    }
    res |= TWDR;
  }
  /* Send Stop */
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 < TWSTO);
  return res;
}

void PM::init(void) {
  TWBR = 0x48;
  write(REG_CTRLA, 0b00001110); //calibrate on demand, continuous scan, sense only
  write(REG_NADC, 0x80); //12 bit mode

  long abc = read(REG_NADC,1);
 // Serial.print("REG_NADC : 0b");
  //Serial.print(abc, BIN);

 // Serial.println("  PM init done !");

}

bool PM::dataReady(void) {
  int abc = read(REG_ADC_STATUS,1);
  //Serial.print("STATUS : 0b");
 // Serial.print(abc, BIN);
  return (abc && 0x83);
}

void PM::readInputPower(void) {
  long p = read(REG_P1,3);
  input.p = p * 0.031875;
  long i = read(REG_I1,2)>>4;
  input.i = i * 1.25;
  long v = read(REG_S1,2)>>4;
  input.v *= v * 0.0255;
}

void PM::displayPower(Power_T pt) {
  /*Serial.print(pt.v, 2);
  Serial.print("V\t");
  Serial.print(pt.i, 1);
  Serial.print("mA\t");
  Serial.print(pt.p, 1);
  Serial.print("mW\t");*/
}

void PM::readOutputPower(void) {

  long p = read(REG_P2,3);
  output.p = p * 0.031875;
  long i = read(REG_I2,2)>>4;
  output.i = i * 1.25;
  long v = read(REG_S2,2)>>4;
  output.v *= v * 0.0255;
}
