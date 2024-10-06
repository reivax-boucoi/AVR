// SimpleRx - the slave or the receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10

const byte slaveAddress[5] = "XBEA0";

RF24 radio(CE_PIN, CSN_PIN);

uint8_t dataToSend[4] = "OKx"; // etage (4 bits), altitude (18 bits), battery voltage (10 bits)
//[3]: eeeeaaaa [2]: aaaaaaaa [1]: aaaaaabb [0]: bbbbbbbb
uint8_t etage = 0;
uint32_t alti = 0;
uint16_t batt = 0;


unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 250;

void setup() {

    Serial.begin(115200);

    Serial.println("SimpleRx Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setRetries(3, 5); // delay, count
    radio.openWritingPipe(slaveAddress);
}

//=============

void loop() {
    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis) {
        send();
        if (++etage > 15)etage = 0;
        if (++batt > 1023)batt = 0;
        alti += 16;
        if (alti > 262143)alti = 0;
        prevMillis = millis();
    }
}


void send() {

    bool rslt;
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );

    Serial.print("Data Sent\t");
    Serial.print(dataToSend[3]);
    Serial.print(dataToSend[2]);
    Serial.print(dataToSend[1]);
    Serial.print(dataToSend[0]);
    Serial.print('\t\t');
    Serial.print(etage);
    Serial.print('\t');
    Serial.print(batt);
    Serial.print('\t');
    Serial.print(alti);
    updateMessage();
    if (rslt) {
        Serial.println("\tAcknowledge received");
    }
    else {
        Serial.println("\tTx failed");
    }
}

void updateMessage() {
    dataToSend[3] = (etage << 4) | ((alti >> 14) & 0x0F);
    dataToSend[2] = alti >> 6;
    dataToSend[1] = ((alti << 2) & 0xFC) | ((batt >> 8) & 0x03);
    dataToSend[0] = batt;
}
