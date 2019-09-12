#ifndef PM_H
#define PM_H

#define LTC2992_addr 0x6F //0x66=mass response

#define REG_CTRLA 0x00
#define REG_CTRLB 0x01
#define REG_ADC_STATUS 0x32
#define REG_NADC 0x04
#define REG_GPIO_STATUS 0x95
#define REG_GPIO4_CONTROL 0x97

#define REG_P1 0x05
#define REG_MAX_P1 0x08
#define REG_MIN_P1 0x0B

#define REG_I1 0x14
#define REG_MAX_I1 0x16
#define REG_MIN_I1 0x18

#define REG_S1 0x1E
#define REG_MAX_S1 0x20
#define REG_MIN_S1 0x22

#define REG_G1 0x28
#define REG_MAX_G1 0x2A
#define REG_MIN_G1 0x2C


#define REG_P2 0x37
#define REG_MAX_P2 0x3A
#define REG_MIN_P2 0x3D

#define REG_I2 0x46
#define REG_MAX_I2 0x48
#define REG_MIN_I2 0x4A

#define REG_S2 0x50
#define REG_MAX_S2 0x52
#define REG_MIN_S2 0x54

#define REG_G2 0x5A
#define REG_MAX_G2 0x5C
#define REG_MIN_G2 0x5E

#define REG_G3 0x64
#define REG_MAX_G3 0x66
#define REG_MIN_G3 0x68

#define REG_G4 0x6E
#define REG_MAX_G4 0x70
#define REG_MIN_G4 0x72

#define REG_ISUM 0x78
#define REG_MAX_ISUM 0x7A
#define REG_MIN_ISUM 0x7C

#define REG_PSUM 0x82
#define REG_MAX_PSUM 0x85
#define REG_MIN_PSUM 0x88


#include <Arduino.h>
#include <Wire.h>


class PM {
  struct Power_T{
    float v;
    float i;
    float p;
  };
  public :
  Power_T input,output;
    void init(void);
    bool dataReady(void);
    void readInputPower(void);
    void readOutputPower(void);
    void displayPower(Power_T pt);
private :
      void write(int address, int data);
      int read(int address);
    };
#endif
