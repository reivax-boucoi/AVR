#include "PM.h"



int write(int address, int data){
    return -1;
}
long read(int address, int l){
    return -1;
}

void PM::init(void) {
    TWBR = 0x48;
    write(REG_CTRLA, 0b00001110); //calibrate on demand, continuous scan, sense only
    write(REG_NADC, 0x80); //12 bit mode
    
    long abc = read(REG_NADC,1);
    // Serial.print("REG_NADC : 0b");
    //Serial.print(abc, BIN);
    
    // Serial.println("  PM init done !");
    
}

bool PM::dataReady(void) {
    int abc = read(REG_ADC_STATUS,1);
    //Serial.print("STATUS : 0b");
    // Serial.print(abc, BIN);
    return (abc && 0x83);
}

void PM::readInputPower(void) {
    long p = read(REG_P1,3);
    input.p = p * 0.031875;
    long i = read(REG_I1,2)>>4;
    input.i = i * 1.25;
    long v = read(REG_S1,2)>>4;
    input.v *= v * 0.0255;
}

void PM::displayPower(Power_T pt) {
    /*Serial.print(pt.v, 2);
     * Serial.print("V\t");
     * Serial.print(pt.i, 1);
     * Serial.print("mA\t");
     * Serial.print(pt.p, 1);
     * Serial.print("mW\t");*/
}

void PM::readOutputPower(void) {
    
    long p = read(REG_P2,3);
    output.p = p * 0.031875;
    long i = read(REG_I2,2)>>4;
    output.i = i * 1.25;
    long v = read(REG_S2,2)>>4;
    output.v *= v * 0.0255;
}
