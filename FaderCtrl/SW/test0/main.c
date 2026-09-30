#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include "buttons.h"
#include "pwm_sled.h"
#include "gpio_static.h"
#define TOGGLE_DELAY_MS 1000

#define TIMER_PRESCALE_DIV  8UL
#define TIMER_PER           ((F_CPU / TIMER_PRESCALE_DIV / 1000UL) * DEBOUNCE_SAMPLE_MS - 1)

volatile uint16_t adc_result;


static void clock_init(void){
    OSC.CTRL |= OSC_RC32MEN_bm;
    while (!(OSC.STATUS & OSC_RC32MRDY_bm));
    CCP = CCP_IOREG_gc;
    CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
}

static void adc_init(void){
    PORTB.PIN4CTRL = PORT_ISC_INPUT_DISABLE_gc;        /* disable the digital input buffer on PB4 */
    ADCB.CTRLB     = ADC_RESOLUTION_12BIT_gc;          /* unsigned, 12-bit, no free-run */
    ADCB.REFCTRL   = ADC_REFSEL_AREFB_gc;              /* external reference on AREFB (PB0) */
    ADCB.PRESCALER = ADC_PRESCALER_DIV32_gc;           /* 24 MHz / 32 = 750 kHz */
    ADCB.CH0.CTRL    = ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCB.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN4_gc;          /* PB4 */
    ADCB.CH0.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc;
    EVSYS.CH0MUX   = EVSYS_CHMUX_TCC0_OVF_gc;          /* TCC0 overflow -> event channel 0 */
    ADCB.EVCTRL    = ADC_EVSEL_0123_gc | ADC_EVACT_CH0_gc;  /* event triggers ADC channel 0 */
    ADCB.CTRLA     = ADC_ENABLE_bm;
}

static void timer_init(void){
    TCC0.CTRLA = TC_CLKSEL_DIV8_gc;
    TCC0.PER = TIMER_PER;
    TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;
    PMIC.CTRL |= PMIC_LOLVLEN_bm;
}

ISR(ADCB_CH0_vect){
    adc_result = ADCB.CH0.RES;                         /* flag is cleared automatically */
    if(adc_result>4090){
        LED1_ON();
    }else{
        LED1_OFF();
    }
    if(adc_result<200){
        LED0_ON();
    }else{
        LED0_OFF();
    }
    pwm_sled_set_duty(0, (float)adc_result*100.0/4096.0);

}

ISR(TCC0_OVF_vect){
    buttons_sample();
}

int main(void){
    gpio_static_init();
    clock_init();
    adc_init();
    buttons_init();
    timer_init();
    pwm_sled_init();
    sei();

    while (1){

        for (uint8_t i = 0; i < NUM_BUTTONS; i++){
            button_state_t st;
            if (buttons_changed(i, &st)){
                if(st == BTN_PRESSED){
                    switch(i){
                        case 0:
                            LED1_TOGGLE();

                            break;
                        case 1:
                            LED2_TOGGLE();
                            break;
                        case 2:
                            LED3_TOGGLE();
                            break;
                    }
                    //LED0_TOGGLE();
                }
            }
        }
    }
}
