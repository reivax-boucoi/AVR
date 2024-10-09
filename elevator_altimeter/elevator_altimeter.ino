#include "Alti.h"

#define NRF_CE PIN_PA0
#define NRF_SCK PIN_PA1
#define NRF_MISO PIN_PA2
#define NRF_IRQ PIN_PA4
#define NRF_MOSI PIN_PA5
#define NRF_CSN PIN_PA6

#define MPL_INT1 PIN_PC2//INT0/PCINT14

#define USER_SW PIN_PC0 //PCINT12 ->PCI2
#define Batt_sense 0 //PIN_PA3->ADC0
#define LED0 PIN_PC4
#define LED1 PIN_PC5

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const byte slaveAddress[5] = "XBEA";
uint8_t dataToSend[4] = "OKx"; // etage (4 bits), altitude (18 bits), battery voltage (10 bits)
//[3]: eeeeaaaa [2]: aaaaaaaa [1]: aaaaaabb [0]: bbbbbbbb
RF24 radio(NRF_CE, NRF_CSN);

uint8_t log_mode = 0;
uint16_t batt_v = 0;
Alt_Data alt_data;

void setup() {
    pinMode(LED0, OUTPUT);
    pinMode(LED1, OUTPUT);
    pinMode(USER_SW, INPUT_PULLUP);
    pinMode(MPL_INT1, INPUT);
    pinMode(NRF_CE, OUTPUT);
    pinMode(NRF_SCK, OUTPUT);
    pinMode(NRF_MOSI, OUTPUT);
    pinMode(NRF_CSN, OUTPUT);
    pinMode(NRF_IRQ, INPUT);
    pinMode(NRF_MISO, INPUT);
    Serial.begin(9600);
    Serial.println("Reboot");
    delay(50);
    MPL_init();

    MCUCR = (1 << ISC01); //Interrupt on falling edge of INTO
    GIMSK |= (1 << PCIE2) | (1 << INT0);
    PCMSK2 |= (1 << PCINT12);
    analogReference(DEFAULT);

    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setRetries(3, 5); // delay, count
    radio.openWritingPipe(slaveAddress);
    Serial.println("Initialized radio !");

}

void loop() {
    if (alt_data.newDataReady) {
        digitalWrite(LED1, HIGH);
        alt_data.newDataReady = false;
        batt_v = analogRead(Batt_sense);// ;
        uint32_t alti;
        uint16_t temp;
        MPL_readAltTemp(&alti, &temp);
        uint8_t etage = alt_data.getEtage(alti);
        switch (log_mode) {
            case 0://All: alt,temp,batt
                Serial.print(alti / 16.0); //in meters
                Serial.print(",");
                Serial.print(temp / 16.0); //in degrees C
                Serial.print(",");
                Serial.println(batt_v / 1024.0 * 2.5 * 2.0);
                break;
            case 1://alt only
                Serial.println(alti / 16.0); //in meters
                break;
            case 2://temp only
                Serial.println(temp / 16.0); //in degrees C
                break;
            case 3://batt only
                Serial.println(batt_v / 1024.0 * 2.5 * 2.0);
                break;
            default:
                break;
        }
        digitalWrite(LED1, LOW);
        digitalWrite(LED0, HIGH);

        dataToSend[3] = (etage << 4) | ((alti >> 14) & 0x0F);
        dataToSend[2] = alti >> 6;
        dataToSend[1] = ((alti << 2) & 0xFC) | ((batt_v >> 8) & 0x03);
        dataToSend[0] = batt_v;
        send();
        digitalWrite(LED0, LOW);
    }
    //GOTO sleep
}

ISR(PCINT2_vect) {

    if (!digitalRead(USER_SW)) {
        if (++log_mode > 4)log_mode = 0;
        Serial.print("Mode :");
        Serial.println(log_mode);
    }
}

ISR(INT0_vect) {
    alt_data.newDataReady = true;
}

void send() {

    bool rslt;
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );
    //    Serial.print("Data Sent\t");
    //    Serial.print(dataToSend[3]);
    //    Serial.print(dataToSend[2]);
    //    Serial.print(dataToSend[1]);
    //    Serial.print(dataToSend[0]);
    //    Serial.print('\t\t');
    //    Serial.print(etage);
    //    Serial.print('\t');
    //    Serial.print(batt);
    //    Serial.print('\t');
    //    Serial.print(alti);
    if (rslt) {
        Serial.println("\tAcknowledge received");
    }
    else {
        Serial.println("\tTx failed");
    }
}
