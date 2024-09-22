#include "RF24.h"
#include "SPI.h"

#define DIG1 4//PIN_PB4
#define DIG2 5//PIN_PB5
#define DIG3 0//PIN_PB0
#define LEDS 6//PIN_PB6
#define USER_SW 7//PIN_PB7
#define PWR_EN 5//PIN_PF5
#define NRF_IRQ PIN_PF4
#define NRF_CSN 23 //PIN_PF0
#define NRF_CE 22 //PIN_PF1


#define LED_Red 0
#define LED_Yellow 2
#define LED_Green 1

#define DASH 16
#define DP (1<<2)
#define PWRDOWN_TIMEOUT 30.0 //seconds
#define PWRDOWN_CNT_MAX (PWRDOWN_TIMEOUT/0.002048) //(16MHz/256/128=488Hz / 2ms)

//    0-9,    A-F,    Dash
const uint8_t segMap[17] = {250, 34, 185, 171, 99, 203, 219, 162, 251, 235, 243, 91, 216, 59, 217, 209, 1};

uint8_t displayVal[4] = {0, DASH, 0, 0};//Last digit=LEDs
volatile uint8_t displayCurrentDigit = 0;
uint8_t pButton = 1 << 7;

uint8_t cnt = 0;
volatile uint16_t powerUP_time = 0;

uint8_t displayVariable = 1;
uint8_t altitude = 0;
uint8_t etage = 0;
float batt_level = 1.23;

RF24 radio(NRF_CE, NRF_CSN); //CE,CSN
const uint64_t pipe = 0xE6E6E6E6E6E6;

void setup() {

    Serial.begin(9600);
    radio.begin(); // Start the NRF24L01
    radio.openWritingPipe(pipe); // Get NRF24L01 ready to transmit
    radio.stopListening();
    
    DDRD = 0xFF; //segments as outputs
    PORTD = 0x00; //segments off

    DDRF |= (1 << PWR_EN);
    PORTF |= (1 << PWR_EN);

    DDRB |= (1 << DIG1) | (1 << DIG2) | (1 << DIG3) | (1 << LEDS);
    DDRB &= ~(1 << USER_SW);//USER_SW is input
    PORTB &= ~(1 << DIG2);
    PORTB |= (1 << DIG1) | (1 << DIG3) | (1 << USER_SW);//USER_SW enable internal pullup

    TCCR4B = 0b0001000; //prescaler at CLK/256
    TIMSK4 = 0b100; //enable timer 4 overflow interrupt
    sei();
    Serial.println("hello");
}
void loop() {
    delay(1);
    if (++cnt == 0) {
        radio.write(&etage, sizeof(etage));
        switch (displayVariable) {
            case 0:
                setDisplayInt(altitude++);
                break;
            case 1:
                setDisplayInt(etage++);
                etage = etage % 16;
                break;
            case 2:
                setDisplayFLoat(batt_level);
                batt_level += 0.01;
                break;
            default:
                displayVal[0] = segMap[DASH];
                displayVal[1] = segMap[DASH];
                displayVal[2] = segMap[DASH];
                displayVal[3] = (1 << LED_Red);
                break;
        }
    }


    if (pButton != (PINB & (1 << USER_SW) )) {
        pButton = PINB & (1 << USER_SW);
        if (pButton) {
            powerUP_time = 0;
            if (displayVariable < 3) {//DO not allow to change displayed variabel is still searching signal
                if (++displayVariable > 2)displayVariable = 0;
                displayVal[3] = (1 << displayVariable);
                Serial.println(displayVariable);
            }
        }
    }
    if (powerUP_time > PWRDOWN_CNT_MAX) {
        Serial.println("Shutting down");
        PORTF &= ~(1 << PWR_EN);
        while (1);
    }
}

void setDisplayInt(uint16_t val) {
    if (val > 999)val = 999;
    displayVal[2] = segMap[val % 10];
    if (val > 9) {
        val /= 10;
        displayVal[1] = segMap[val % 10];
        if (val > 9) {
            val /= 10;
            displayVal[0] = segMap[val % 10];
        } else {
            displayVal[0] = 0;
        }
    } else {
        displayVal[1] = 0;
        displayVal[0] = 0;
    }
}

void setDisplayFLoat(float val) {
    if (val < 0)val = -val;
    if (val > 100.0) {
        setDisplayInt(int(val));
        return;
    }
    if (val > 9.99) {
        displayVal[1] = DP;
        displayVal[0] = segMap[int(val / 10.0)];
    } else {
        displayVal[1] = 0;
        displayVal[0] = segMap[int(val) % 10] | DP;
        val *= 10.0;
    }

    displayVal[1] |= segMap[int(val) % 10];
    val *= 10.0;
    displayVal[2] = segMap[int(val) % 10];

}

void displayMutliplex() {
    switch (displayCurrentDigit) {
        case 0:
            PORTB &= ~(1 << DIG1);
            PORTB |= (1 << DIG2) | (1 << DIG3);
            break;
        case 1:
            PORTB &= ~(1 << DIG2);
            PORTB |= (1 << DIG1) | (1 << DIG3);
            break;
        case 2:
            PORTB &= ~(1 << DIG3);
            PORTB |= (1 << DIG1) | (1 << DIG2);
            break;
    }
    PORTD = displayVal[displayCurrentDigit++];
    if (displayCurrentDigit > 2)displayCurrentDigit = 0;
    if (displayVal[3] & (1 << displayCurrentDigit)) {
        PORTB |= (1 << LEDS);
    } else {
        PORTB &= ~(1 << LEDS);
    }
}

ISR(TIMER4_OVF_vect) {
    displayMutliplex();
    powerUP_time++;
}
