#ifndef PWM_HBRIDGE_H
#define PWM_HBRIDGE_H
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include <avr/pgmspace.h>

/* ---- Pin assignment: TCC0 + AWEXC, complementary pairs ----
 * PC0/PC1 = PWM1A/PWM1B (H-bridge 1, TCC0 channel A)
 * PC2/PC3 = PWM2A/PWM2B (H-bridge 2, TCC0 channel B)
 * PC4/PC5 = PWM3A/PWM3B (H-bridge 3, TCC0 channel C)
 * PC6/PC7 = PWM4A/PWM4B (H-bridge 4, TCC0 channel D)*/
#define HBRIDGE_PORT      PORTC
#define HBRIDGE_ALL_bm    (PIN0_bm|PIN1_bm|PIN2_bm|PIN3_bm|PIN4_bm|PIN5_bm|PIN6_bm|PIN7_bm)

#define HBRIDGE_PWM_FREQ_HZ     22000UL
#define HBRIDGE_PWM_PER         ((F_CPU/HBRIDGE_PWM_FREQ_HZ)-1)

#define POS_DEADBAND     10
#define POS_OUT_MAX      (int32_t)(HBRIDGE_PWM_PER/2)

#define POS_KP_Q8       150L   /* position gain */
#define POS_KD_Q8       200L   /* velocity damping, brakes the arrival */
#define POS_OUT_MIN     340L   /* creep drive */
#define POS_RAMP        48L    /* extra drive added per call while stalled */
#define POS_VEL_STALL   3      /* counts/call: slower than this = stalled */

void pwm_hbridge_init(void);
void pwm_hbridge_set_duty(uint8_t channel, uint16_t duty);
void pwm_hbridge_OFF(uint8_t channel);
void pwm_hbridge_setPos(uint8_t channel, uint16_t targetPos, uint16_t currentPos);

#endif
