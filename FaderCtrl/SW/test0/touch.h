#ifndef TOUCH_H
#define TOUCH_H

#include <stdint.h>

/* ---- Pin assignment ---- */
#define TOUCH_PORT     PORTF
#define TOUCH1_bm      PIN0_bm   /* PF0, TP1, fader 1 */
#define TOUCH2_bm      PIN1_bm   /* PF1, TP2, fader 2 */
#define TOUCH3_bm      PIN4_bm   /* PF4, TP3, fader 3 */
#define TOUCH4_bm      PIN6_bm   /* PF6, TP4, fader 4 */
#define NUM_TOUCH_PADS 4

/* ---- Test constants ----
 * Sensing method: self-capacitance charge-time measurement using the
 * pin's internal weak pull-up as the charge path and a free-running
 * hardware timer (TCF0) as the time base, latched by a PORT pin-change
 * interrupt on the rising edge - HW peripherals do the timing, no
 * CPU busy-wait polling loop measures the RC time itself. */
#define TOUCH_DISCHARGE_US     10     /* pad discharge time before each measurement */
#define TOUCH_TIMEOUT_TICKS    2000   /* abort a stuck measurement after this many timer counts */
#define TOUCH_THRESHOLD        150    /* raw charge-time count above which touch is declared -- TUNE ON HARDWARE */
#define TOUCH_DEBOUNCE_COUNT   3      /* consecutive samples required to accept a state change */
#define TOUCH_SCAN_PERIOD_MS   20     /* time between successive pad measurements (one pad at a time) */

typedef enum { TOUCH_RELEASED = 0, TOUCH_TOUCHED = 1 } touch_state_t;

void touch_init(void);
/* Charges + measures the next pad in round-robin order. Call every
 * TOUCH_SCAN_PERIOD_MS (blocks until that pad's measurement completes
 * or times out). */
void touch_scan_next(void);
/* Returns 1 and clears the flag if pad `id` (0..3) changed state since last call */
uint8_t touch_changed(uint8_t id, touch_state_t *new_state);

#endif
