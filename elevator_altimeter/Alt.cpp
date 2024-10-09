#include "Alti.h"
#include "Arduino.h"
#include <Wire.h>

#define ALTI_ADDR 0x60 //DEC 96
#define ALTI_REG_WHO_AM_I 0x0C
#define ALTI_REG_PT_DATA_CFG  0x13
#define ALTI_REG_CTRL1 0x26
#define ALTI_REG_CTRL4 0x29
#define ALTI_REG_CTRL5 0x2A

Alt_Data::Alt_Data() {
    alt_RdC = ALT_RdC_INIT;
    newDataReady = true;

}

uint8_t Alt_Data::getEtage(uint32_t alt) {
    alti = alt;

    if (alti < alt_RdC) {
        alt_RdC = alti;
#ifdef DEBUG_Alt
        Serial.print("rs down, ");
#endif
    } else if (alti > (alt_RdC + 15 * FLOOR_HEIGHT)) {
        alt_RdC = alti - (15 * FLOOR_HEIGHT);
#ifdef DEBUG_Alt
        Serial.print("rs up, ");
#endif
    }

    etage = (alti - alt_RdC) / FLOOR_HEIGHT;
#ifdef DEBUG_Alt
    Serial.print(alti);
    Serial.print(',');
    Serial.print(alt_RdC);
    Serial.print(',');
    Serial.println(etage);
#endif
    return etage;
}


void MPL_init() {
    Wire.begin();
    Wire.beginTransmission(ALTI_ADDR);
    Wire.write(ALTI_REG_CTRL1);
    Wire.write(0xB8);//0b10111000);//Altimeter mode, OSR=0b111(128,512ms)
    Wire.endTransmission();

    Wire.beginTransmission(ALTI_ADDR);
    Wire.write(ALTI_REG_PT_DATA_CFG);
    Wire.write(0x07);//0b00000111);//DREM=PDEFE=TDEFE=1: data ready event on new temperature or altitude readings.
    Wire.endTransmission();

    Wire.beginTransmission(ALTI_ADDR);
    Wire.write(ALTI_REG_CTRL4);
    Wire.write(0x80);//INT_EN_DRDY=1 enable data ready interrupt
    Wire.endTransmission();

    Wire.beginTransmission(ALTI_ADDR);
    Wire.write(ALTI_REG_CTRL5);
    Wire.write(0x80);//INT_EN_DRDY=1 route Data ready interrupt to INT1
    Wire.endTransmission();

    Wire.beginTransmission(ALTI_ADDR);
    Wire.write(ALTI_REG_CTRL1);
    Wire.write(0xB9);//0b10111001);//Altimeter mode, OSR=0b111(128,512ms), Active
    Wire.endTransmission();
}
uint32_t MPL_readAlt() {

    Wire.beginTransmission(ALTI_ADDR);
    Wire.write(0); //STATUS register (reads DR_STATUS reg (0x06))
    uint8_t n = 4;
    uint32_t alti = 0; // in 16*meters
    Wire.requestFrom(ALTI_ADDR, n);//request n bytes from slave device, free up bus after transmission
    for (uint8_t i = 0; i < n; i++) {
        uint8_t d = Wire.read();
        if (i == 0) {
#ifdef DEBUG_Alt
            if (d == 0x0E) { // 0x0E: 0b00001110 : new temp & altitude data is available.
                Serial.println("New MPL data available");
            } else if ( d & 0xF0) { //PTOW-POW-TOW some data was overwritten
                Serial.println("MPL data was overwritten");
            } else { //no new data available
                Serial.println("No new MPL data available");
            }
#endif
        } else {
            alti = (alti << 8) | d;
        }
    }
    alti = alti >> 4;
}

void MPL_readAltTemp(uint32_t *alt, uint16_t *temp) {

    Wire.beginTransmission(ALTI_ADDR);
    Wire.write(0); //STATUS register (reads DR_STATUS reg (0x06))
    uint8_t n = 6;
    uint32_t alti = 0; // in 16*meters
    uint16_t tempe = 0; //in 16*degrees
    Wire.requestFrom(ALTI_ADDR, n);//request n bytes from slave device, free up bus after transmission
    for (uint8_t i = 0; i < n; i++) {
        uint8_t d = Wire.read();
        if (i == 0) {
            if (d == 0x0E) { // 0x0E: 0b00001110 : new temp & altitude data is available.
                //Serial.print(",d");
            } else if ( d & 0xF0) { //PTOW-POW-TOW some data was overwritten
                //Serial.print(",ok");
            } else { //no new data available

            }
        } else if (i < 4) {
            alti = (alti << 8) | d;
        } else {
            tempe = (tempe << 8) | d; //temp is 8MSBs=int value, 8LSBs=[4 decimal, 0b0000].
        }
    }
    *alt = alti >> 4;
    *temp = tempe >> 4;
}
