#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>

#define BTN_PORT     PORTB
#define BTN1_bm      PIN1_bm   /* PB1 */
#define BTN2_bm      PIN2_bm   /* PB2 */
#define BTN3_bm      PIN3_bm   /* PB3 */
#define BTN_ALL_bm   (BTN1_bm | BTN2_bm | BTN3_bm)
#define NUM_BUTTONS  3

#define DEBOUNCE_SAMPLE_MS   10     /* periodic sample tick period */
#define DEBOUNCE_COUNT       4     /* consecutive samples required to accept a state change */
#define TIMER_PRESCALE_DIV  1024UL
#define TIMER_PER           (((F_CPU / TIMER_PRESCALE_DIV) / 1000UL) * DEBOUNCE_SAMPLE_MS - 1)

typedef enum { BTN_RELEASED = 0, BTN_PRESSED = 1 } button_state_t;

void timer_init(void);
void buttons_init(void);
void buttons_sample(void);
uint8_t buttons_changed(uint8_t id, button_state_t *new_state);

#endif
