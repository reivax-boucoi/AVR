#include <avr/io.h>
#include "buttons.h"

static const uint8_t btn_bm[NUM_BUTTONS] = { BTN1_bm, BTN2_bm, BTN3_bm };

static uint8_t stable_state[NUM_BUTTONS];   /* 1 = released, 0 = pressed (active-low) */
static uint8_t counter[NUM_BUTTONS];
static volatile uint8_t changed_flag[NUM_BUTTONS];
static volatile button_state_t changed_to[NUM_BUTTONS];

void timer_init(void){
    TCC1.CTRLA = TC_CLKSEL_DIV1024_gc;
    TCC1.PER = 311;//TIMER_PER;
    TCC1.INTCTRLA = TC_OVFINTLVL_LO_gc;
    PMIC.CTRL |= PMIC_LOLVLEN_bm;
}

void buttons_init(void){
    BTN_PORT.DIRCLR = BTN_ALL_bm;
    BTN_PORT.PIN1CTRL = PORT_OPC_PULLUP_gc;
    BTN_PORT.PIN2CTRL = PORT_OPC_PULLUP_gc;
    BTN_PORT.PIN3CTRL = PORT_OPC_PULLUP_gc;

    for (uint8_t i = 0; i < NUM_BUTTONS; i++){
        stable_state[i] = 1; /* idle = released (pulled up) */
        counter[i] = 0;
        changed_flag[i] = 0;
    }
}

void buttons_sample(void){
    uint8_t raw_port = BTN_PORT.IN;

    for (uint8_t i = 0; i < NUM_BUTTONS; i++){
        uint8_t raw = (raw_port & btn_bm[i]) ? 1 : 0;

        if (raw != stable_state[i]){
            if (++counter[i] >= DEBOUNCE_COUNT){
                stable_state[i] = raw;
                counter[i] = 0;
                changed_flag[i] = 1;
                changed_to[i] = raw ? BTN_RELEASED : BTN_PRESSED; /* active-low */
            }
        } else {
            counter[i] = 0;
        }
    }
}

uint8_t buttons_changed(uint8_t id, button_state_t *new_state){
    if (id >= NUM_BUTTONS) return 0;
    if (!changed_flag[id]) return 0;
    changed_flag[id] = 0;
    *new_state = changed_to[id];
    return 1;
}
