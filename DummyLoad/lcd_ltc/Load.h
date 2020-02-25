#ifndef LOAD_H
#define LOAD_H

#include <Arduino.h>
#include <Wire.h>


#define DAC_ADDR   0x61

#define DAC_REG    0x00
#define VREF_REG   0x08
#define PD_REG     0x09
#define GAIN_REG   0x0A
#define WL_REG     0x0B
#define NVDAC_REG  0x10
#define NVVREF_REG 0x18
#define NVPD_REG   0x19
#define NVGAIN_REG 0x20

#define DAC_CAL_COEFF 783.387978142 // Iout=2*Vref*DACVal/4096/.2*R1/(R1+R2), DACVal=Iout*.2*4096/2/Vref*(R1+R2)/R1
#define LOAD_REG_THRESHOLD 0.001



class Load {
  public:
    void init(void);
    void regulate(float current);
    void on(float current);
    void off(void);
    bool onState;

  private:
    void DAC_set(int val);
    void DAC_set(void);
    void DAC_on(void);
    void DAC_off(void);
    float setCurrent;
    int actualDACVal;
};

#endif