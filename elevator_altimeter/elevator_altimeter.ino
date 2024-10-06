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

#include "RF24.h"

uint8_t log_mode = 0;
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

    MCUCR = (1<<ISC01);//Interrupt on falling edge of INTO
    GIMSK |= (1 << PCIE2) | (1<<INT0);
    PCMSK2 |= (1 << PCINT12);
    analogReference(DEFAULT);

}

void loop() {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED0, LOW);
    delay(200);
    digitalWrite(LED1, LOW);
    digitalWrite(LED0, HIGH);
    delay(200);

    float batt_v = analogRead(Batt_sense) / 1024.0 * 3.2;
    uint32_t alti;
    uint16_t temp;
    MPL_readAltTemp(&alti,&temp);
    uint8_t etage=alt_data.getEtage(alti);
    switch (log_mode) {
        case 0://All: alt,temp,batt
            Serial.print(alti / 16.0); //in meters
            Serial.print(",");
            Serial.print(temp / 16.0); //in degrees C
            Serial.print(",");
            Serial.println(batt_v);
            break;
        case 1://alt only
            Serial.println(alti / 16.0); //in meters
            break;
        case 2://temp only
            Serial.println(temp / 16.0); //in degrees C
            break;
        case 3://batt only
            Serial.println(batt_v);
            break;
        default:
            break;
    }
    //GOTO sleep
}

ISR(PCINT2_vect) {

    if (!digitalRead(USER_SW)) {
        if (++log_mode > 3)log_mode = 0;
        Serial.print("Mode :");
        Serial.println(log_mode);
    }
}

ISR(INT0_ISR){
    Serial.println("MPL interrupt");
}
