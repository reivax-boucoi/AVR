#include "Display.h"

void initDisplay(void) {
  DISPLAYREG |= (1 << CLK) | (1 << DATA) | (1 << LATCH);
  writeData(DECODE, 0x3F);//D0-D5 decode D6 no decode D7 off
  writeData(SCAN_LIMIT, 0x06); // digits D0-D6 in use, leds on D6, D7 off
  writeData(INTENSITY, 0x0F);//min 0x01 max 0x0F
  writeData(POWER, 1);
}


void sendByte (uint8_t databyte) {
  uint8_t mask;
  for (mask = 0x80; mask; mask >>= 1) {
    if (databyte & mask) {
      DISPLAYPORT |= (1 << DATA);
    } else {
      DISPLAYPORT &= ~(1 << DATA);
    }
    DISPLAYPORT |= (1 << CLK);
    DISPLAYPORT &= ~(1 << CLK);
  }
}

void setRange(uint8_t range) {
  switch (range) {
    case 0:
      writeData(LEDS, HZ);
      break;
    case 1:
      writeData(LEDS, KHZ);
      break;
    case 2:
      writeData(LEDS, MHZ);
      break;
    default:
      displayNumber(1000000);
      break;
  }
}
void displayNumber(uint32_t number){
	displayNumberDP(number,7);
}
void displayNumberDP(uint32_t number, uint8_t dp_digit) {
  clearDisplay();
  if (number == 0) {
    writeData(D0, 0);
    return;
  } else if (number > 999999 || number < 0) {
    writeData(D3, LH);
    writeData(D2, LE);
    writeData(D1, LL);
    writeData(D0, LP);
    return;
  }
  uint8_t i = NBDIGITS;
  do {
	  if(i!=dp_digit){
		    writeData(i--,(number % 10));
	  }else{
		    writeData(i--,(1<<7)|(number % 10));
	  }
    number /= 10;
  } while (number);
}

void clearDisplay() {
  uint8_t i = NBDIGITS;
  do {
    writeData(i, BLANK);
  } while (--i);
}

void writeData(uint8_t reg, uint8_t data) {
  DISPLAYPORT &= ~(1 << LATCH);
  sendByte(reg);
  sendByte(data);
  DISPLAYPORT |= (1 << LATCH);
}
