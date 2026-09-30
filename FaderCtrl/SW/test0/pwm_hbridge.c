#include <avr/io.h>
#include "pwm_hbridge.h"

void pwm_hbridge_init(void){
    HBRIDGE_PORT.DIRSET = HBRIDGE_ALL_bm;

    TCC0.CTRLB = TC0_CCAEN_bm | TC0_CCBEN_bm | TC0_CCCEN_bm | TC0_CCDEN_bm | TC_WGMODE_SS_gc;
    TCC0.PER = HBRIDGE_PWM_PER;
    TCC0.CCA = HBRIDGE_PWM_PER / 2;   /* start centered (stopped) */
    TCC0.CCB = HBRIDGE_PWM_PER / 2;
    TCC0.CCC = HBRIDGE_PWM_PER / 2;
    TCC0.CCD = HBRIDGE_PWM_PER / 2;

    /* Enable dead-time insertion + complementary LS/HS output on all 4
     * channels (NOT pattern-generation mode - that's for BLDC commutation
     * and is not what we want here). */
    AWEXC.CTRL = AWEX_DTICCAEN_bm | AWEX_DTICCBEN_bm | AWEX_DTICCCEN_bm | AWEX_DTICCDEN_bm;
    AWEXC.OUTOVEN = 0xFF;   /* let AWEX drive PC0..PC7 */
    AWEXC.DTLS = HBRIDGE_DEADTIME_CYCLES;
    AWEXC.DTHS = HBRIDGE_DEADTIME_CYCLES;

    TCC0.CTRLA = TC_CLKSEL_DIV1_gc;   /* start the timer */
}


void pwm_hbridge_set_duty(uint8_t channel, uint8_t duty_percent){
    if (duty_percent > 100) duty_percent = 100;
    uint16_t cc = (uint16_t)(((uint32_t)(HBRIDGE_PWM_PER + 1) * duty_percent) / 100);

    switch (channel){
        case 0: TCC0.CCA = cc; break;
        case 1: TCC0.CCB = cc; break;
        case 2: TCC0.CCC = cc; break;
        case 3: TCC0.CCD = cc; break;
        default: break;
    }
}
