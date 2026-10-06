#include <avr/io.h>
#include <util/atomic.h>
#include "pwm_hbridge.h"

/* Two pins per channel: channel n -> PC(2n) and PC(2n+1) */
#define CH_PINS_bm(ch)  ((uint8_t)(0x03u << (2u * (ch))))

void pwm_hbridge_init(void){
    /* Outputs low, and AWEX does not drive them yet (all channels start OFF
     * until the first pwm_hbridge_set_duty call). */
    HBRIDGE_PORT.OUTCLR = HBRIDGE_ALL_bm;
    HBRIDGE_PORT.DIRSET = HBRIDGE_ALL_bm;

    TCC0.CTRLB = TC0_CCAEN_bm | TC0_CCBEN_bm | TC0_CCCEN_bm | TC0_CCDEN_bm | TC_WGMODE_SS_gc;
    TCC0.PER = HBRIDGE_PWM_PER;
    TCC0.CCA = HBRIDGE_PWM_PER / 2;   /* start centered (stopped) */
    TCC0.CCB = HBRIDGE_PWM_PER / 2;
    TCC0.CCC = HBRIDGE_PWM_PER / 2;
    TCC0.CCD = HBRIDGE_PWM_PER / 2;

    /* Keep the AWEX dead-time unit enabled ONLY because it is what produces
     * the complementary LS/HS pair from one compare channel (without DTICCxEN
     * the same signal goes to both pins). Dead-time values are 0, so no
     * dead-time is inserted. */
    AWEXC.CTRL = AWEX_DTICCAEN_bm | AWEX_DTICCBEN_bm | AWEX_DTICCCEN_bm | AWEX_DTICCDEN_bm;
    AWEXC.DTLS = 0;
    AWEXC.DTHS = 0;
    AWEXC.OUTOVEN = 0x00;   /* all outputs OFF (port-controlled, low) */

    TCC0.CTRLA = TC_CLKSEL_DIV1_gc;   /* start the timer */
}

void pwm_hbridge_OFF(uint8_t channel){
    if (channel > 3) return;
    uint8_t bm = CH_PINS_bm(channel);

    /* Make sure the pins will be low once AWEX releases them, then release. */
    HBRIDGE_PORT.OUTCLR = bm;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        AWEXC.OUTOVEN &= (uint8_t)~bm;
    }
}

void pwm_hbridge_set_duty(uint8_t channel, uint16_t duty){
    if (channel > 3) return;
    if (duty >=HBRIDGE_PWM_PER) duty = HBRIDGE_PWM_PER-1;

    /* Update the compare value first, so the outputs come back with the
     * requested duty and not a stale one. */
    switch (channel){
        case 0: TCC0.CCA = duty; break;
        case 1: TCC0.CCB = duty; break;
        case 2: TCC0.CCC = duty; break;
        case 3: TCC0.CCD = duty; break;
    }

    /* Re-enable AWEX drive of this channel's pins (no-op if already enabled). */
    uint8_t bm = CH_PINS_bm(channel);
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        AWEXC.OUTOVEN |= bm;
    }
}

int32_t pwm_hbridge_setPos(uint8_t channel, uint16_t targetPos, uint16_t currentPos){
    static int32_t iterm;

    int32_t err = (int32_t)targetPos - (int32_t)currentPos;

    if (err < POS_DEADBAND && err > -POS_DEADBAND){
        pwm_hbridge_OFF(channel);
        iterm = 0;
        return 0;
    }

    int32_t i_new = iterm + (int32_t)POS_KI_Q8 * err;
    int32_t u = (((int32_t)POS_KP_Q8 * err) + i_new)>>8;

    uint8_t sat = 0;
    if (u >  700){
        u =  700;
        sat = (err > 0);
    }
    if (u < -700){
        u = -700;
        sat = (err < 0);
    }
    if (!sat) iterm = i_new;

    pwm_hbridge_set_duty(channel,POS_OUT_MAX-u);
    return u;
}
