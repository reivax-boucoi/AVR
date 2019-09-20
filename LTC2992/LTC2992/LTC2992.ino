
#define BLUE_LED 5
#define YELL_LED 6
#define BUTTON 2



#include <Wire.h>
#include "PM.h"

enum displayMode { IN,OUT,INOUT,FULL,EXTENDED};
enum scanMode { OFF,CONTINUOUS};

displayMode mode=FULL;
scanMode=OFF;
int delay=500;
unsigned long timeStamp;

void displayHelp(void){
    Serial.println("\nPower Monitor based on LTC2992\r\n");
    Serial.println("Scanmode:");
    Serial.println("\ts = single scan");
    Serial.println("\tc = continuous scan (Press any key to quit)");
    Serial.println("\tp = increase delay between samples");
    Serial.println("\tm = decrease delay between samples");
    Serial.println("Output data:"));
    Serial.println("\ti = Display only input data");
    Serial.println("\to = Display only output data");
    Serial.println("\tt = Display basin in/out data");
    Serial.println("\tf = Display full data");
    Serial.println("\te = Display extended data");
    Serial.println("\n\th = help - this page");
    Serial.println();
}
void makeMeasurement(void){
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
    }else {
        Serial.println("Data not ready");
    }
}
void incrementDelay(bool inc){
    if(inc){
        delay*=1.5;
    }else{
        delay/=1.5;
    }
    if(delay<100)delay=100;
    if(delay>50000)delay=50000;
}
PM pm;


void setup() {
    pinMode(BLUE_LED,OUTPUT);
    pinMode(YELL_LED,OUTPUT);
    pinMode(BUTTON,INPUT);
    
    Serial.begin(115200);
    Serial.println("\r\n\r\n\r\nStarting PM\r\n\r\n");
    Wire.begin();
    pm.init();
    
}


void loop() {
    if (Serial.available() > 0) {
        byte data = Serial.read();
        
        if(scanMode=CONTINUOUS){
            scanMode=OFF;
        }
        switch(data){
            case 'h':
                displayHelp();
                break;
            case 's':
                makeMeasurement();
                break;
            case 'c':
                scanMode=CONTINUOUS;
                makeMeasurement();
                timeStamp=millis();
                break;
            case 'i':
                mode=IN;
                break;
            case 'o':
                mode=OUT;
                break;
            case 't':
                mode=INOUT;
                break;
            case 'f':
                mode=FULL;
                break;
            case 'e':
                mode=EXTENDED;
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
    if((timeStamp+delay)<millis()){
        makeMeasurement();
        timeStamp=millis();
    }
    
}


