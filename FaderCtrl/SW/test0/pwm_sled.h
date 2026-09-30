#ifndef PWM_SLED_H
#define PWM_SLED_H

#include <stdint.h>

/* ---- Pin assignment: TCD0, independent (non-complementary) PWM ----
 * PD0=SLED1 (ch A), PD1=SLED2 (ch B), PD2=SLED3 (ch C), PD3=SLED4 (ch D) */
#define SLED_PORT        PORTD
#define SLED_ALL_bm      (PIN0_bm|PIN1_bm|PIN2_bm|PIN3_bm)

/* ---- Test constants ---- */
#define SLED_PWM_FREQ_HZ   1000UL
#define SLED_PWM_PER       ((F_CPU / 64UL / SLED_PWM_FREQ_HZ) - 1)   /* 499 -> exactly 1kHz @ 32MHz/64 */

void pwm_sled_init(void);
void pwm_sled_set_duty(uint8_t channel /*0..3*/, uint8_t duty_percent /*0..100, 0=off*/);

#endif
