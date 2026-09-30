#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "touch.h"

static const uint8_t touch_bm[NUM_TOUCH_PADS] = { TOUCH1_bm, TOUCH2_bm, TOUCH3_bm, TOUCH4_bm };

static volatile uint8_t active_pad = 0;
static volatile uint16_t start_count = 0;
static volatile uint16_t last_count[NUM_TOUCH_PADS];
static volatile uint8_t capture_done = 0;

static uint8_t stable_state[NUM_TOUCH_PADS];
static uint8_t counter[NUM_TOUCH_PADS];
static volatile uint8_t changed_flag[NUM_TOUCH_PADS];
static volatile touch_state_t changed_to[NUM_TOUCH_PADS];

void touch_init(void){
    /* free-running time base; does not drive any output pin */
    TCF0.CTRLA = TC_CLKSEL_DIV1_gc;
    TCF0.PER = 0xFFFF;

    TOUCH_PORT.PIN0CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_RISING_gc;
    TOUCH_PORT.PIN1CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_RISING_gc;
    TOUCH_PORT.PIN4CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_RISING_gc;
    TOUCH_PORT.PIN6CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_RISING_gc;

    TOUCH_PORT.INT0MASK = TOUCH1_bm | TOUCH2_bm | TOUCH3_bm | TOUCH4_bm;
    TOUCH_PORT.INTCTRL = PORT_INT0LVL_LO_gc;
    PMIC.CTRL |= PMIC_LOLVLEN_bm;

    for (uint8_t i = 0; i < NUM_TOUCH_PADS; i++){
        stable_state[i] = TOUCH_RELEASED;
        counter[i] = 0;
        changed_flag[i] = 0;
    }
}

ISR(PORTF_INT0_vect){
    if (!capture_done){
        last_count[active_pad] = TCF0.CNT - start_count;
        capture_done = 1;
    }
}

void touch_scan_next(void){
    active_pad = (active_pad + 1) % NUM_TOUCH_PADS;
    uint8_t bm = touch_bm[active_pad];

    /* discharge the pad fully */
    TOUCH_PORT.DIRSET = bm;
    TOUCH_PORT.OUTCLR = bm;
    _delay_us(TOUCH_DISCHARGE_US);

    /* release to input; the internal pull-up starts charging the pad,
     * the PORTF_INT0 ISR captures TCF0.CNT on the rising edge */
    capture_done = 0;
    start_count = TCF0.CNT;
    TOUCH_PORT.DIRCLR = bm;

    while (!capture_done){
        if ((uint16_t)(TCF0.CNT - start_count) > TOUCH_TIMEOUT_TICKS){
            last_count[active_pad] = TOUCH_TIMEOUT_TICKS;
            capture_done = 1;
        }
    }

    uint8_t raw = (last_count[active_pad] > TOUCH_THRESHOLD) ? TOUCH_TOUCHED : TOUCH_RELEASED;
    if (raw != stable_state[active_pad]){
        if (++counter[active_pad] >= TOUCH_DEBOUNCE_COUNT){
            stable_state[active_pad] = raw;
            counter[active_pad] = 0;
            changed_flag[active_pad] = 1;
            changed_to[active_pad] = raw;
        }
    } else {
        counter[active_pad] = 0;
    }
}

uint8_t touch_changed(uint8_t id, touch_state_t *new_state){
    if (id >= NUM_TOUCH_PADS) return 0;
    if (!changed_flag[id]) return 0;
    changed_flag[id] = 0;
    *new_state = changed_to[id];
    return 1;
}
