#include <Wire.h>
#include "i2c.h"

#include "i2c_MMA8451.h"
MMA8451 mma8451;

uint16_t acc_max=0;
uint16_t acc_min=0;

void setup()
{
    Serial.begin(115200);

    Serial.print("Probe MMA8451: ");
    if (mma8451.initialize(200, 2)) Serial.println("Sensor found!");
    else
    {
        Serial.println("Sensor missing");
        while (1) {};
    }
}

void loop()
{
    uint8_t _buf[2];
    i2c.read(0x1C, 0x03, _buf, 6);//0x1=X axis, 0x3=Y axis
    uint16_t accA = (_buf[0] << 6) | ((_buf[1] >> 2) & 0x63);
    //if (accA > 8191) accA -= 16384;
    //float acc=float(accA)/4096.0;//4096=2g range, 1024=8g range
    acc_min=min(accA,acc_min+10);
    acc_max=max(accA,acc_max-10);
    
    Serial.print(acc_max);
    Serial.print(',');
    Serial.print(acc_min);
    Serial.print(',');
    Serial.println(accA);
    delay(50);

}


/** <

    Program size:
    A1.0.5:
    A1.5.7: 6992b
    A1.6.3: 6988b / 494b
*/
