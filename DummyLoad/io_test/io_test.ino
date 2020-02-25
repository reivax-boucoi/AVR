
#include <SimpleRotary.h>
#include <AceButton.h>
using namespace ace_button;


#define BTN_BACK_PIN A0
#define BTN1_PIN A1
#define ENC_A_PIN 12
#define ENC_B_PIN 13
#define ENC_SW_PIN 11
#define LEDR1_PIN 9
#define LEDB1_PIN 10
#define LEDR2_PIN A2
#define LEDB2_PIN A3


bool BTN1_state = true;
bool BTN0_state = true;
bool loadOn=false;

void BTN1_check(void) {
  if (digitalRead(BTN1_PIN) != BTN1_state) {
    BTN1_state = !BTN1_state;
    if (!BTN1_state) {
      Serial.println("BTN1 pressed !");
      loadOn=!loadOn;
      digitalWrite(LEDB2_PIN,loadOn);
    }
  }
}
void BTN0_check(void) {
  if (digitalRead(BTN_BACK_PIN) != BTN0_state) {
    BTN0_state = !BTN0_state;
    if (!BTN0_state) {
      Serial.println("BTN0 pressed !");
    }
  }
}
//Rotary encoder : 13 12, btn 11
SimpleRotary rotary(ENC_A_PIN, ENC_B_PIN, ENC_SW_PIN);


void setup() {
  Serial.begin(115200);
  pinMode(ENC_A_PIN, INPUT);
  pinMode(ENC_B_PIN, INPUT);
  pinMode(ENC_SW_PIN, INPUT);
  pinMode(BTN_BACK_PIN, INPUT);
  pinMode(BTN1_PIN, INPUT);
  pinMode(LEDR1_PIN,OUTPUT);
  pinMode(LEDB1_PIN,OUTPUT);
  pinMode(LEDR2_PIN,OUTPUT);
  pinMode(LEDB2_PIN,OUTPUT);
  digitalWrite(LEDR1_PIN,0);
  digitalWrite(LEDB1_PIN,0);
  digitalWrite(LEDR2_PIN,0);
  digitalWrite(LEDB2_PIN,0);
  Serial.println("Electronic Load Started !");

}

void loop() {
  BTN1_check();
  BTN0_check();
  byte i;
  i = rotary.rotate();
  if (i == 1) {
    Serial.println("Next");
  } else if (i == 2) {
    Serial.println("Prev");
  }
  if (rotary.push()) {
    Serial.println("Enter");
  }
}
