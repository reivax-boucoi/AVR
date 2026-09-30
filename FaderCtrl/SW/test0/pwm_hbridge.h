#ifndef PWM_HBRIDGE_H
#define PWM_HBRIDGE_H
#include <stdint.h>

/* ---- Pin assignment: TCC0 + AWEXC, complementary pairs ----
 * PC0/PC1 = PWM1A/PWM1B (H-bridge 1, TCC0 channel A)
 * PC2/PC3 = PWM2A/PWM2B (H-bridge 2, TCC0 channel B)
 * PC4/PC5 = PWM3A/PWM3B (H-bridge 3, TCC0 channel C)
 * PC6/PC7 = PWM4A/PWM4B (H-bridge 4, TCC0 channel D)
 * Each pair is ONE dead-time-inserted complementary output driven from a
 * single compare register, not two independently-settable duty cycles. */
#define HBRIDGE_PORT      PORTC
#define HBRIDGE_ALL_bm    (PIN0_bm|PIN1_bm|PIN2_bm|PIN3_bm|PIN4_bm|PIN5_bm|PIN6_bm|PIN7_bm)

/* ---- Test constants ---- */
#define HBRIDGE_PWM_FREQ_HZ     22000UL
#define HBRIDGE_PWM_PER         ((F_CPU / HBRIDGE_PWM_FREQ_HZ) - 1)   /* 1453 @ 32MHz -> ~21.99kHz */
#define HBRIDGE_DEADTIME_CYCLES 8    /* ~250ns @ 32MHz, applied to both LS and HS edges */

/* duty_percent: 0..100. Locked-antiphase drive: 50% = stopped/centered,
 * <50%/>50% = direction + speed. */
void pwm_hbridge_init(void);
void pwm_hbridge_set_duty(uint8_t channel /*0..3*/, uint8_t duty_percent /*0..100*/);

#endif
