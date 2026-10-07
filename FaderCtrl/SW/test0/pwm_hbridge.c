#include <avr/io.h>
#include <util/atomic.h>
#include "pwm_hbridge.h"

#define CH_PINS_bm(ch)  ((uint8_t)(0x03u << (2u * (ch))))

void pwm_hbridge_init(void){
    HBRIDGE_PORT.OUTCLR = HBRIDGE_ALL_bm;
    HBRIDGE_PORT.DIRSET = HBRIDGE_ALL_bm;

    TCC0.CTRLB = TC0_CCAEN_bm | TC0_CCBEN_bm | TC0_CCCEN_bm | TC0_CCDEN_bm | TC_WGMODE_SS_gc;
    TCC0.PER = HBRIDGE_PWM_PER;
    TCC0.CCA = HBRIDGE_PWM_PER / 2;
    TCC0.CCB = HBRIDGE_PWM_PER / 2;
    TCC0.CCC = HBRIDGE_PWM_PER / 2;
    TCC0.CCD = HBRIDGE_PWM_PER / 2;

    AWEXC.CTRL = AWEX_DTICCAEN_bm | AWEX_DTICCBEN_bm | AWEX_DTICCCEN_bm | AWEX_DTICCDEN_bm;
    AWEXC.DTLS = 0;
    AWEXC.DTHS = 0;
    AWEXC.OUTOVEN = 0x00;

    TCC0.CTRLA = TC_CLKSEL_DIV1_gc;//Start timer
}

void pwm_hbridge_OFF(uint8_t channel){
    if (channel > 3) return;
    uint8_t bm = CH_PINS_bm(channel);

    HBRIDGE_PORT.OUTCLR = bm;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        AWEXC.OUTOVEN &= (uint8_t)~bm;
    }
}

void pwm_hbridge_set_duty(uint8_t channel, uint16_t duty){
    if (channel > 3) return;
    if (duty >=HBRIDGE_PWM_PER) duty = HBRIDGE_PWM_PER-1;
    switch (channel){
        case 0: TCC0.CCA = duty; break;
        case 1: TCC0.CCB = duty; break;
        case 2: TCC0.CCC = duty; break;
        case 3: TCC0.CCD = duty; break;
    }
    uint8_t bm = CH_PINS_bm(channel);
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        AWEXC.OUTOVEN |= bm;
    }
}

void pwm_hbridge_setPos(uint8_t channel, uint16_t targetPos, uint16_t currentPos){
    static int32_t boost;
    static uint16_t prevPos;

    int32_t err = (int32_t)targetPos - (int32_t)currentPos;
    int32_t vel = (int32_t)currentPos - (int32_t)prevPos;
    prevPos = currentPos;
    int32_t u = 0;

    if (err < POS_DEADBAND && err > -POS_DEADBAND){
        pwm_hbridge_OFF(channel);
        boost = 0;

        printf_P(PSTR("%u,%u,%ld,%ld\r\n"), targetPos, currentPos, err, u);
        return;
    }
    int32_t dir = (err > 0) ? 1 : -1;
    if (vel > POS_VEL_STALL || vel < -POS_VEL_STALL) boost = 0;   /* moving */
        else boost += dir * POS_RAMP;                                 /* stalled: push harder */

            u = ((POS_KP_Q8 * err - POS_KD_Q8 * vel) >> 8) + dir * POS_OUT_MIN + boost;
    if (u >= POS_OUT_MAX) u =  POS_OUT_MAX-1;
    if (u <= -POS_OUT_MAX) u = 1-POS_OUT_MAX;
    pwm_hbridge_set_duty(channel, POS_OUT_MAX - u);
    printf_P(PSTR("%u,%u,%ld,%ld\r\n"), targetPos, currentPos, err, u);
}
