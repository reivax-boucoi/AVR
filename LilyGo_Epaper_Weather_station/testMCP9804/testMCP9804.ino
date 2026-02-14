
#include <Wire.h>
#include "epd_driver.h"
#define I2C_SDA 18
#define I2C_SCL 17
#define MCP9804_ADDR 0x18
void setup() {

    Wire.begin(I2C_SDA, I2C_SCL);
    epd_init();
    epd_poweron();      // Switch on EPD display

    Serial.begin(115200);
    while (!Serial);
    Serial.println("\nI2C Scanner");
}

void loop() {
    delay(250);//Default MCP9804 resultion=0.0625°C, beware Tconv=250ms
    Serial.println(getI2CTemperature());
}
