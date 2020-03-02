#include <ButtonDebounce.h>

//PB4 - ADC2 : analog in
//PB0 : Button, active low
//PB1 : LED, active HIGH
//PB3 : Output PWM
void buttonChanged(int event);
bool state = true;
int val = 10;
ButtonDebounce button(PB0, 100);
void setup() {
  pinMode(PB1, OUTPUT);
  pinMode(PB2, OUTPUT);
  pinMode(PB4, INPUT);
  pinMode(PB0, INPUT_PULLUP);
  button.setCallback(buttonChanged);
}

void loop() {
  button.update();
  if(state){
    val = analogRead(2)/10;
    digitalWrite(PB2, HIGH);
    delayMicroseconds(val);
    digitalWrite(PB2, LOW);
    delayMicroseconds(10000 - val);
  }
}

void buttonChanged(int event) {
  if (event) {
    state = 1 - state;
    digitalWrite(PB1, state);
  }
  if (!state)digitalWrite(PB2,LOW);
}

