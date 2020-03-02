#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include <SimpleRotary.h>
#include "Screen.h"
#include "Menu.h"

#define VERSION_NUMBER "0.1"

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

void BTN1_check(void);
void BTN0_check(void);
#define VOLTAGE_MAX 99.0
#define RSET_MAX 1000.0
#define RSET_MIN 0.2
#define CURRENT_MAX 5.0
#define POWER_MAX   500.0
#define TEMP_MAX    70.0
#define TEMP_HOT    40.0

//Rotary encoder : 13 12, btn 11
SimpleRotary rotary(ENC_A_PIN, ENC_B_PIN, ENC_SW_PIN);


const int LCD_RS = 8, LCD_E = 7, LCD_D4 = 6, LCD_D5 = 5, LCD_D6 = 4, LCD_D7 = 3;
LiquidCrystal lcd = LiquidCrystal(8, 7, 6, 5, 4, 3);

float i_meas = 5.1;
float v_meas = 24;
float p_meas = 0.24;
float iset_load = 2.5;
float pset_load = 10;
float rset_load = 0.01;
float temp = 24.0;
float v_load = 99;
float idisp_load = 4.9;
float pdisp_load = 50.2;

float setting1=0;
float setting2=0;

Value i_measV(&i_meas, 10, 0, 4);
Value v_measV(&v_meas, 2, 0, 4);
Value p_measV(&p_meas, 2, 1, 4);

Value i_setV(&iset_load, 5, 0, 4,0.0,CURRENT_MAX);
Value p_setV(&pset_load, 5, 0, 4,0.0,POWER_MAX);
Value r_setV(&rset_load, 5, 0, 4,0.2,20.0);

Value tempV(&temp, 14, 0, 2);
Value v_loadV(&v_load, 2, 1, 4);

Value p_loadV(&pdisp_load, 10, 1, 4);
Value i_loadV(&idisp_load, 10, 1, 4);

Value setting1V(&setting1,2,1,2,0,99);
Value setting2V(&setting2,9,1,2,0,99);

const String myStr=String("CR R      ")+String((char)244)+String(" T   ");

Screen meterScreen("V       I      A", "P      W        ",NULL, &v_measV,   &i_measV, &p_measV);

Screen CCloadScreen("CC I      A T   ", "V       P      W", &i_setV, &tempV,   &v_loadV, &p_loadV);
Screen CPloadScreen("CP P      W T   ", "V       I      A", &p_setV, &tempV,   &v_loadV, &i_loadV);
Screen CRloadScreen(myStr.c_str(), "V       I      A", &r_setV, &tempV,   &v_loadV, &i_loadV);//Ω

Screen settingsScreen("Settings        ", "V      V       ", &setting1V, &setting2V, NULL, NULL);

MenuItem meterItem("Meter", 0, 0, &meterScreen);
MenuItem loadItem("Load", 7, 0, &CCloadScreen, &CPloadScreen, &CRloadScreen, NULL);
MenuItem settingsItem("Settings", 7, 1, &settingsScreen);

Menu mainMenu(&meterItem, &loadItem, &settingsItem, NULL);



void setup() {
  Serial.begin(115200);

  lcd.begin(16, 2);
  lcd.print("Electronic Load");
  lcd.setCursor(0,1);
  lcd.print("0-100V 0-5A v");
  lcd.print(VERSION_NUMBER);
  delay(3000);
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
  
  mainMenu.show();

  Serial.println("Electronic Load Started !");
}
void loop() {

  BTN1_check();
  BTN0_check();
  byte i;
  i = rotary.rotate();
  if (i == 1) {
    mainMenu.next();
  } else if (i == 2) {
    mainMenu.prev();
  }
  if (rotary.push()) {
    mainMenu.enter();
  }
}


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
        mainMenu.back();
      Serial.println("BTN0 pressed !");
    }
  }
}
