#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DISPLAYREG DDRA
#define DISPLAYPORT PORTA
#define CLK PORTA2  // to CLK
#define DATA  PORTA0  // to CS
#define LATCH  PORTA1 // to DATA

#define DECODE  0x09 // regs
#define INTENSITY 0x0A
#define SCAN_LIMIT  0x0B
#define POWER 0x0C
#define TEST  0x0F
#define LEDS  0x07
#define D0  0x06
#define D1  0x05
#define D2  0x04
#define D3  0x03
#define D4  0x02
#define D5  0x01

#define BLANK 0xF // segs
#define DASH  0xA
#define LH  0xC
#define LE  0xB
#define LL  0xD
#define LP  0xE
#define MHZ 0x20  //B 
#define KHZ 0x40  //A
#define HZ  0x10  //C

#define NBDIGITS 6

void sendByte (uint8_t databyte);
void writeData(uint8_t reg, uint8_t data);
void clearDisplay();
void displayNumber(volatile long number);
void setRange(uint8_t range);
void initDisplay(void);

  volatile unsigned long i = 0;
  volatile uint16_t counts = 0;
  volatile uint32_t total = 0;
int main(void) {
	_delay_ms(2000);
	initDisplay();
	_delay_ms(1000);
	setRange(1);
	displayNumber(-1);
	uint8_t state=0;
	DDRB &=~(1<<PINB2);//high speed input // low division
	TCCR0B |=(1<<CS02)|(1<<CS00);
	TIMSK0|=(1<<TOIE0);
	TCCR1B |=(1<<CS12)|(1<<CS11)|(1<<CS10); // check soldering for slow input (currently PB2) to go to T1
	TIMSK1 |=(1<<TOIE1);
	sei();
	while (1)  {

	}
}
ISR(TIM1_OVF_vect){ // input overflow
	counts++;
}

ISR(TIM0_OVF_vect){
	i++;
	//uint16_t div = 0.524288;//F_CPU/(1024*256*1)UL; // F_CPU / (prescaler*T/C res°*UpdateSlowDown) 
	total+=TCNT1+65536*counts;
	if(i%32==0){
		displayNumber(total/(61*8));
		total=0;
		i=0;
	}
	counts=0;
	TCNT1=0;

}

void initDisplay(void) {
  DISPLAYREG |= (1 << CLK) | (1 << DATA) | (1 << LATCH);
  writeData(DECODE, 0x3F);//D0-D5 decode D6 no decode D7 off
  writeData(SCAN_LIMIT, 0x06); // digits D0-D6 in use, leds on D6, D7 off
  writeData(INTENSITY, 0x08);//min 0x01 max 0x0F
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

void displayNumber(volatile long number) {
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
    writeData(i--, number % 10);
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
