
#define BLUE_LED 5
#define YELL_LED 6
#define BUTTON 2



#include <Wire.h>
#include "PM.h"

enum displayMode { 
  IN,OUT,INOUT,FULL,EXTENDED};
enum scanMode { 
  OFF,CONTINUOUS};

displayMode mode=FULL;
scanMode sm=OFF;
int del=500;
bool bstate=true;
unsigned long timeStamp;
PM pm;

void displayHelp(void){
  Serial.println("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\nPower Monitor based on LTC2992\r\n");
  Serial.println("Scanmode:");
  Serial.println("\ts = single scan");
  Serial.println("\tc = continuous scan (Press any key to quit)");
  Serial.print("\t\tCurrent delay between samples : ");
  Serial.print(del);
  Serial.println("ms");
  Serial.println("\tp = increase delay between samples");
  Serial.println("\tm = decrease delay between samples");
  Serial.println("Output data:");
  Serial.println("\ti = Display only input data");
  Serial.println("\to = Display only output data");
  Serial.println("\tt = Display basic in/out data");
  Serial.println("\tf = Display full data");
  Serial.println("\te = Display extended data");
  Serial.println("Output format:");
  Serial.println("\tv = output to CSV");
  Serial.println("\tb = output to human readable format");
  Serial.println("\n\th = help - this page");
  Serial.println();
}
void makeMeasurement(void){
  digitalWrite(BLUE_LED,HIGH);
  if (pm.dataReady()) {
    pm.readInputPower();
    pm.readOutputPower();
    if(mode !=OUT){
      Serial.print("Input : ");
      pm.displayPower(pm.input);
      Serial.print("\t");
    }
    if(mode !=IN){
      Serial.print("Output : ");
      pm.displayPower(pm.output);
    }
    if(mode == FULL){
      pm.calculateEff();
      pm.displayEff(true);
    }
    Serial.print("\r\n");
  }
  else {
    Serial.println("Data not ready");
  }
  digitalWrite(BLUE_LED,LOW);
}
void incrementDelay(bool inc){
  if(inc){
    del*=1.5;
  }
  else{
    del/=1.5;
  }
  if(del<100)del=100;
  if(del>50000)del=50000;
  Serial.print("Delay : ");
  Serial.print(del);
  Serial.println("ms");
}



void setup() {
  pinMode(BLUE_LED,OUTPUT);
  pinMode(YELL_LED,OUTPUT);
  pinMode(BUTTON,INPUT);

  Serial.begin(115200);
  Serial.println("\r\n\r\n\r\nStarting PM\r\n\r\n");
  displayHelp();
  Wire.begin();
  pm.init();

}


void loop() {
  if (Serial.available() > 0) {
    byte data = Serial.read();

    if(sm==CONTINUOUS){
      sm=OFF;
      digitalWrite(YELL_LED,LOW);
    }
    else{
      switch(data){
      case 'h':
        displayHelp();
        break;
      case 's':
        makeMeasurement();
        break;
      case 'c':
        sm=CONTINUOUS;
        digitalWrite(YELL_LED,HIGH);
        Serial.println("Starting continuous acquisition");
        makeMeasurement();
        timeStamp=millis();
        break;
      case 'i':
        mode=IN;
        Serial.println("Set acquisition to IN");
        break;
      case 'o':
        mode=OUT;
        Serial.println("Set acquisition to OUT");
        break;
      case 't':
        mode=INOUT;
        Serial.println("Set acquisition to IN/OUT");
        break;
      case 'f':
        mode=FULL;
        Serial.println("Set acquisition to FULL");
        break;
      case 'e':
        mode=EXTENDED;
        Serial.println("Set acquisition to EXTENDED");
        break;
      case 'p':
        incrementDelay(true);
        break;
      case 'm':
        incrementDelay(false);
        break;
      default:
        displayHelp();
      }
    }
  }
  if(digitalRead(BUTTON)!=bstate){
    bstate=!-bstate;
    if(bstate){
      makeMeasurement();
    } 
  }
  if(sm==CONTINUOUS && (timeStamp+del)<millis()){
    makeMeasurement();
    timeStamp=millis();
  }
}





