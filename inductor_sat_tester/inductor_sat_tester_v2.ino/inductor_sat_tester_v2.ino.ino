#include <AceButton.h>
#include <AdjustableButtonConfig.h>
#include <ButtonConfig.h>
using namespace ace_button;


//PB4 - ADC2 : analog in
//PB0 : Button, active low
//PB1 : LED, active HIGH
//PB3 : Output PWM
bool state = true;
unsigned int val = 10;
volatile unsigned int scale = 1;
AceButton button((const)PB0);
void handleEvent(AceButton*, uint8_t, uint8_t);

void setup() {
  pinMode(PB1, OUTPUT);
  pinMode(PB2, OUTPUT);
  pinMode(PB4, INPUT);
  pinMode(PB0, INPUT_PULLUP);
  digitalWrite(PB1, HIGH);
  ButtonConfig* buttonConfig = button.getButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
}

void loop() {
  button.check();
  if (state) {
    val = analogRead(2) / 10;
    digitalWrite(PB2, HIGH);
    delayMicroseconds(val * scale);
    digitalWrite(PB2, LOW);
    if(scale>1){
      delay((10000-val)*scale/1000);
    }else{
      delayMicroseconds((10000 - val)*scale);
    }
  }
}
void handleEvent(AceButton*, uint8_t eventType, uint8_t buttonState) {
  switch (eventType) {
    case AceButton::kEventPressed:
      break;
    case AceButton::kEventClicked:
      if (state == false) {
        state = true;
        digitalWrite(PB1, HIGH);
      } else {
        scale *= 2;
        if (scale > 32)scale = 1;
      }
      break;
    case AceButton::kEventLongPressed:
      digitalWrite(PB1, LOW);
      digitalWrite(PB2, LOW);
      state = false;
      break;
    case AceButton::kEventReleased:
      break;
  }
}

