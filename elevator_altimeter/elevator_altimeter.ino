
#define NRF_CE PIN_PA0
#define NRF_SCK PIN_PA1
#define NRF_MISO PIN_PA2
#define NRF_IRQ PIN_PA4
#define NRF_MOSI PIN_PA5
#define NRF_CSN PIN_PA6

#define ALTI_ADDR 0x60 //DEC 96
#define ALTI_REG_WHO_AM_I 0x0C
#define ALTI_REG_CTRL1 0x26

#define USER_SW PIN_PC0 //PCINT12 ->PCI2
#define Batt_sense 0 //PIN_PA3->ADC0
#define LED0 PIN_PC4
#define LED1 PIN_PC5

#include <Wire.h>
#include "RF24.h"

uint8_t log_mode = 0;

void setup() {
    pinMode(LED0, OUTPUT);
    pinMode(LED1, OUTPUT);
    pinMode(USER_SW, INPUT_PULLUP);
    GIMSK |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT12);
    pinMode(NRF_CE, OUTPUT);
    pinMode(NRF_SCK, OUTPUT);
    pinMode(NRF_MOSI, OUTPUT);
    pinMode(NRF_CSN, OUTPUT);
    pinMode(NRF_IRQ, INPUT);
    pinMode(NRF_MISO, INPUT);
    Serial.begin(9600);
    Serial.println("Reboot");
    delay(500);
    Wire.begin();
    Wire.beginTransmission(ALTI_ADDR);
    Wire.write(0x26);//CTRL_REG1
    Wire.write(0xB8);//0b10111000);//Altimeter mode, OSR=0b111(128,512ms)
    Wire.endTransmission();

    Wire.beginTransmission(ALTI_ADDR);
    Wire.write(0x13);//PT_DATA_CFG
    Wire.write(0x07);//0b00000111);//DREM=PDEFE=TDEFE=1: data ready event on new temperature or altitude reeadings.
    Wire.endTransmission();

    Wire.beginTransmission(ALTI_ADDR);
    Wire.write(0x26);//CTRL_REG1
    Wire.write(0xB9);//0b10111001);//Altimeter mode, OSR=0b111(128,512ms), Active
    Wire.endTransmission();

    analogReference(DEFAULT);

}

void loop() {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED0, LOW);
    delay(200);
    digitalWrite(LED1, LOW);
    digitalWrite(LED0, HIGH);
    delay(200);

    float batt_v = analogRead(Batt_sense) / 1024.0 * 3.2;


    Wire.beginTransmission(ALTI_ADDR);
    Wire.write(0);
    uint8_t n = 6;
    uint32_t alti = 0; // in 16*meters
    uint16_t temp = 0; //in 16*degrees
    Wire.requestFrom(ALTI_ADDR, n);
    for (uint8_t i = 0; i < n; i++) {
    uint8_t d = Wire.read();
        if (i == 0) {
            if (d != 0x0E) {
                //Serial.print(",d");
            } else {
                //Serial.print(",ok");
            }
        } else if (i < 4) {
            alti = (alti << 8) | d;
        } else {
            temp = (temp << 8) | d; //temp is 8MSBs=int value, 8LSBs=[4 decimal, 0b0000].
        }
    }
    alti = alti >> 4;
    temp = temp >> 4;
    switch (log_mode) {
    case 0://All: alt,temp,batt
        Serial.print(alti / 16.0); //in meters
            Serial.print(",");
            Serial.print(temp / 16.0); //in degrees C
            Serial.print(",");
            Serial.println(batt_v);
            break;
        case 1://alt only
            Serial.println(alti / 16.0); //in meters
            break;
        case 2://temp only
            Serial.println(temp / 16.0); //in degrees C
            break;
        case 3://batt only
            Serial.println(batt_v);
            break;
        default:
            break;
    }
}

ISR(PCINT2_vect) {

    if (!digitalRead(USER_SW)) {
        if (++log_mode > 3)log_mode = 0;
        Serial.print("Mode :");
        Serial.println(log_mode);
    }
}
