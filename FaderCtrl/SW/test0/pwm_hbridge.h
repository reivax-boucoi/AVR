#ifndef PWM_HBRIDGE_H
#define PWM_HBRIDGE_H
#include <stdint.h>

/* ---- Pin assignment: TCC0 + AWEXC, complementary pairs ----
 * PC0/PC1 = PWM1A/PWM1B (H-bridge 1, TCC0 channel A)
 * PC2/PC3 = PWM2A/PWM2B (H-bridge 2, TCC0 channel B)
 * PC4/PC5 = PWM3A/PWM3B (H-bridge 3, TCC0 channel C)
 * PC6/PC7 = PWM4A/PWM4B (H-bridge 4, TCC0 channel D)
 * Each pair is ONE complementary output driven from a single compare
 * register. Dead-time is NOT generated here: the motor driver handles it. */
#define HBRIDGE_PORT      PORTC
#define HBRIDGE_ALL_bm    (PIN0_bm|PIN1_bm|PIN2_bm|PIN3_bm|PIN4_bm|PIN5_bm|PIN6_bm|PIN7_bm)

#define HBRIDGE_PWM_FREQ_HZ     22000UL
#define HBRIDGE_PWM_PER         ((F_CPU / HBRIDGE_PWM_FREQ_HZ) - 1)   /* 1453 @ 32MHz -> ~21.99kHz */

#define POS_KP_Q8        200L
#define POS_KI_Q8        10L
#define POS_DEADBAND     10
#define POS_OUT_MAX      (HBRIDGE_PWM_PER/2)     /* duty swing around 50 % */


void pwm_hbridge_init(void);

/* Sets duty and (re)enables the PWM outputs of the channel if it was OFF. */
void pwm_hbridge_set_duty(uint8_t channel /*0..3*/, uint16_t duty);

/* Disables PWM on the channel: both complementary outputs forced low.
 * Call pwm_hbridge_set_duty() to re-enable. */
void pwm_hbridge_OFF(uint8_t channel /*0..3*/);
int32_t pwm_hbridge_setPos(uint8_t channel, uint16_t targetPos, uint16_t currentPos);

#endif
