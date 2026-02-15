#include <avr/io.h>
#include <avr/interrupt.h>
#include "peripherals.h"

volatile uint16_t cnt=0;
volatile uint8_t half = 0;       // tracks which half is being sampled

int main(void){
    clk_init();
    GPIO_init();
    timer_init();
    PWM_init();
    adc_init();
    dma_init();
    adc_buffer[0]=409;
    PMIC.CTRL |= PMIC_LOLVLEN_bm;  // Enable low-level interrupts
    sei();                         // Global interrupt enable
    
    while (1){
    }
}

ISR(TCC1_OVF_vect){
    if(++cnt>1000){//(adc_buffer[0]>>2)){
        cnt=0;
        PORTD.OUTTGL = LED_PIN;  // Toggle LED
    }
    //if (half == 0) {
        
        // First half already sampled by DMA → CH0-CH3 = PORTA0-3
        // Prepare next sweep: PORTA4-7
        ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN4_gc;
        ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN5_gc;
        ADCA.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN6_gc;
        ADCA.CH3.MUXCTRL = ADC_CH_MUXPOS_PIN7_gc;
        
        TCC0.CCA = 100+ (0xFFF&adc_buffer[0])>>2;
        TCC0.CCB = 100+ (0xFFF&adc_buffer[1])>>2;
        TCC0.CCC = 100+ (0xFFF&adc_buffer[2])>>2;
        TCC0.CCD = adc_buffer[3]>>2;
        half = 1;
    /*} else {
        // Second half sampled → prepare next sweep: PORTA0-3
        ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
        ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;
        ADCA.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;
        ADCA.CH3.MUXCTRL = ADC_CH_MUXPOS_PIN3_gc;

        half = 0;
    }*/

    ADCA.CTRLA |= ADC_CH0START_bm; // start ADC
}
