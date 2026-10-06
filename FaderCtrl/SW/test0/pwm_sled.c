#include <avr/io.h>
#include "pwm_sled.h"

void pwm_sled_init(void){
    SLED_PORT.DIRSET = SLED_ALL_bm;

    TCD0.CTRLB = TC0_CCAEN_bm | TC0_CCBEN_bm | TC0_CCCEN_bm | TC0_CCDEN_bm | TC_WGMODE_SS_gc;
    TCD0.PER = SLED_PWM_PER;
    TCD0.CCA = SLED_PWM_PER;
    TCD0.CCB = SLED_PWM_PER;
    TCD0.CCC = SLED_PWM_PER;
    TCD0.CCD = SLED_PWM_PER;
    TCD0.CTRLA = TC_CLKSEL_DIV64_gc;
}

void pwm_sled_set_duty(uint8_t channel, uint8_t duty_percent){
    if (duty_percent > 100) duty_percent = 100;
    uint16_t cc = (uint16_t)(((uint32_t)(SLED_PWM_PER + 1) * (100-duty_percent)) / 100);

    switch (channel){
        case 0: TCD0.CCA = cc; break;
        case 1: TCD0.CCB = cc; break;
        case 2: TCD0.CCC = cc; break;
        case 3: TCD0.CCD = cc; break;
        default: break;
    }
}
