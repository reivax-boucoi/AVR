
#define NRF_CE PIN_PA0
#define NRF_SCK PIN_PA1
#define NRF_MISO PIN_PA2
#define NRF_IRQ PIN_PA4
#define NRF_MOSI PIN_PA5
#define NRF_CSN PIN_PA6

#define USER_SW PIN_PC0 //PCINT12 ->PCI2
#define Batt_sense 0 //PIN_PA3->ADC0
#define LED0 PIN_PC4
#define LED1 PIN_PC5

#include <Wire.h>

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
    Wire.begin();
    analogReference(DEFAULT);
    
}

void loop() {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED0, LOW);
    delay(250);
    digitalWrite(LED1, LOW);
    digitalWrite(LED0, HIGH);
    delay(250);

    Serial.println(analogRead(Batt_sense)/1024.0*3.2);


    //Wire.beginTransmission(44); // transmit to device #44 (0x2c)
    //Wire.write(byte(0x00));            // sends instruction byte
    //Wire.write(val);             // sends potentiometer value byte
    //Wire.endTransmission();     // stop transmitting

}

ISR(PCINT2_vect) {

    if (!digitalRead(USER_SW)) {
        Serial.println("Button");
    }
}
