#ifndef PM_H
#define PM_H
#include <Arduino.h>
#include <Wire.h>

#define LTC2991_add 0x48
#define VINATT 1.0
#define CINMUL 1.0
#define VOUTATT 1.0
#define COUTMUL 1.0

#define VSTAB 0.005
#define ISTAB 0.5

class PM {
    struct Power {
      float v;
      float pv;
      float i;
      float pi;
      float p;
      bool valid;
    };

  public:
    Power PowerIn, PowerOut;
    float eff;

    int readPowerIn(void);
    int readPowerOut(void);
    void display(void);
    int init(void);
    void printStatus(void);
    float readTinternal(void) ;
  private:
    void write(int address, int data);
    int read(int address);
    bool validDataAvailable(void);


};
#endif
