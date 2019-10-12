#include <TinyWireM.h>

#define HPC_addr 0x40
#define del 5000

float AOhmScale(float x) { //range 0->10
  return 0.12 * x * x * x - 3.4183 * x * x + 47.51 * x + 1.4929;
}
float BOhmScale(float x) { //range 0->31
  return 0.004 * x * x * x - 0.3545 * x * x + 15.262 * x + 0.3694;
}
float sat(float x) {
  if (x < 0.0)return 0.0;
  if (x > 255.0)return 255.0;
  return x;
}

float getTemperature() {
  TinyWireM.beginTransmission(HPC_addr);
  TinyWireM.write(byte(0xF3));
  TinyWireM.endTransmission();
  delay(75);
  TinyWireM.requestFrom(HPC_addr, 3);
  if (3 <= TinyWireM.available()) {
    uint8_t msb = TinyWireM.read();
    uint8_t lsb = TinyWireM.read();
    uint8_t chk = TinyWireM.read();
    return (((((uint16_t) msb << 8) | (uint16_t) lsb) & 0xFFFC) * (175.72 / 65536.0)) - 46.85;
  }
  return -1;

}

float getHumidity() {
  TinyWireM.beginTransmission(HPC_addr);
  TinyWireM.write(byte(0xF5));//humidity
  TinyWireM.endTransmission();
  delay(75);
  TinyWireM.requestFrom(HPC_addr, 3);
  if (3 <= TinyWireM.available()) {
    uint8_t msb = TinyWireM.read();
    uint8_t lsb = TinyWireM.read();
    uint8_t chk = TinyWireM.read();
    return ((( ((uint16_t) msb << 8) | (uint16_t) lsb) & 0xFFFC) * (125.0 / 65536.0)) - 6.0;
  }
  return -1;
}

float temp=20;
float hum=55;
unsigned long t = 0;
bool state = false;
void setup() {
  DDRB |= (1 << PINB0) | (1 << PINB1) | (1 << PINB2) | (1 << PINB3) ;
  TCCR0A |= (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
  TCCR0B |= (1 << CS00);
  OCR0B = 255;
  delay(10000);
  TinyWireM.begin();
  t = millis();
}

void loop() {
  if (millis() > (t+del)) {
    state = !state;
    PORTB^=(1<<PINB3);
    t = millis();
  }
  if (state) {
    hum=0.95*hum+0.05*getHumidity();
    OCR0B = sat(AOhmScale(hum/10.0));
  } else {
    temp=0.95*temp+0.05*getTemperature();
    OCR0B = sat(BOhmScale(temp));
  }
  delay(75);
}
