#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include <SimpleRotary.h>
#include <Wire.h>
#include "Screen.h"
#include "Menu.h"
#include "Load.h"
#include "PM.h"

#define VERSION_NUMBER "0.1"

#define BTN_BACK_PIN A0
#define BTN1_PIN A1
#define ENC_A_PIN 12
#define ENC_B_PIN 13
#define ENC_SW_PIN 11
#define LEDR1_PIN 9
#define LEDB1_PIN 10

#define REFRESH_TIME 500
#define VOLTAGE_MAX 99.0
#define RSET_MAX 1000.0
#define RSET_MIN 0.2
#define CURRENT_MAX 5.24
#define POWER_MAX   500.0

bool BTN1_state = true;
bool BTN0_state = true;

void BTN1_check(void);
void BTN0_check(void);

//Rotary encoder : 13 12, btn 11
SimpleRotary rotary(ENC_A_PIN, ENC_B_PIN, ENC_SW_PIN);


const int LCD_RS = 8, LCD_E = 7, LCD_D4 = 6, LCD_D5 = 5, LCD_D6 = 4, LCD_D7 = 3;
LiquidCrystal lcd = LiquidCrystal(8, 7, 6, 5, 4, 3);


PM pm;
Load load;

void loadSet(void) {
  Serial.println("Callback called !");
}

void settingsUpdate(void) {

}

//meter values
float i_meas = 0;
float v_meas = 0;
float p_meas = 0;


//load input values
float iset_load = .5;
float pset_load = 1;
float rset_load = 100;

//load output values
float temp = 20.0;
float v_batt = 12.0;
float vdisp_load = 0;
float idisp_load = 0;
float pdisp_load = 0;



//default acquisition time to 0.5s
float setting1 = .5;

long time = 0;

Value i_measV(&i_meas, 10, 0, 4);
Value v_measV(&v_meas, 2, 0, 4);
Value p_measV(&p_meas, 2, 1, 4);

Value i_setV(&iset_load, 5, 0, 4, 0.0, CURRENT_MAX, &loadSet);
Value p_setV(&pset_load, 5, 0, 4, 0.0, POWER_MAX, &loadSet);
Value r_setV(&rset_load, 5, 0, 4, 0.2, 20.0, &loadSet);

Value tempV(&temp, 14, 0, 2);
Value v_battV(&v_batt, 5, 0, 3);
Value v_loadV(&vdisp_load, 2, 1, 4);

Value p_loadV(&pdisp_load, 10, 1, 4);
Value i_loadV(&idisp_load, 10, 1, 4);

//acquisition time, from 0.1 to 10s
Value setting1V(&setting1, 5, 1, 2, 0.1, 10, &settingsUpdate);

const String myStr = String("CR R      ") + String((char)244) + String(" T   ");

Screen meterScreen("V       I      A", "P      W        ", NULL, &v_measV,   &i_measV, &p_measV);

Screen CCloadScreen("CC I      A T   ", "V       P      W", &i_setV, &tempV,   &v_loadV, &p_loadV);
Screen CPloadScreen("CP P      W T   ", "V       I      A", &p_setV, &tempV,   &v_loadV, &i_loadV);
Screen CRloadScreen(myStr.c_str(), "V       I      A", &r_setV, &tempV,   &v_loadV, &i_loadV);//Ω

Screen settingsScreen("Batt     V      ", "Tacq    s       ", &v_battV, &setting1V, NULL, NULL);

MenuItem meterItem("Meter", 0, 0, &meterScreen);
MenuItem loadItem("Load", 7, 0, &CCloadScreen, &CPloadScreen, &CRloadScreen, NULL);
MenuItem settingsItem("Settings", 7, 1, &settingsScreen);

Menu mainMenu(&meterItem, &loadItem, &settingsItem, NULL);



void setup() {
  Serial.begin(115200);
  Wire.begin();
  pm.init();
  load.init();

  pinMode(ENC_A_PIN, INPUT);
  pinMode(ENC_B_PIN, INPUT);
  pinMode(ENC_SW_PIN, INPUT);
  pinMode(BTN_BACK_PIN, INPUT);
  pinMode(BTN1_PIN, INPUT);
  pinMode(LEDR1_PIN, OUTPUT);
  pinMode(LEDB1_PIN, OUTPUT);
  digitalWrite(LEDR1_PIN, 0);
  digitalWrite(LEDB1_PIN, 0);

  lcd.begin(16, 2);
  lcd.print("Electronic Load");
  lcd.setCursor(0, 1);
  lcd.print("0-100V 0-5A v");
  lcd.print(VERSION_NUMBER);
  Serial.println("Electronic Load Started !");
  delay(500);

  mainMenu.show();

  time = millis();
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
  if (time + setting1 * 1000 < millis()) {
    digitalWrite(LEDB1_PIN, HIGH);
    /*if (pm.dataReady()) {
      pm.readInputPower(&v_meas, &i_meas, &p_meas);
      pm.readOutputPower(&vdisp_load, &idisp_load, &pdisp_load);
      v_batt = pm.readBatt();
      if (v_batt < 7.0) {
        Serial.print("Battery Low : ");
        Serial.print(v_batt);
        Serial.println(" !");
      }
      temp = pm.readTemp();
    } else {
      Serial.println("PM not ready !");
    }*/
    mainMenu.refresh();
    digitalWrite(LEDB1_PIN, LOW);
    time = millis();
  }
  load.regulate(iset_load, temp);
}


void BTN1_check(void) {
  if (digitalRead(BTN1_PIN) != BTN1_state) {
    BTN1_state = !BTN1_state;
    if (!BTN1_state) {
      if (load.onState) {
        load.off();
        Serial.println("Load turned off");
      } else {
        load.on(iset_load);
        Serial.println("Load turned on");
      }
    }
  }
}
void BTN0_check(void) {
  if (digitalRead(BTN_BACK_PIN) != BTN0_state) {
    BTN0_state = !BTN0_state;
    if (!BTN0_state) {
      mainMenu.back();
    }
  }
}
