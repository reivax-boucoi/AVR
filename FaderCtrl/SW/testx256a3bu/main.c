#include <avr/io.h>
#include <avr/interrupt.h>
#include "peripherals.h"

volatile uint16_t cnt=0;
volatile uint8_t cnt1=0;
volatile uint8_t half = 0;       // tracks which half is being sampled

int main(void){
    clk_init();
    GPIO_init();
    uart_init();
    timer_init();
    PWM_init();
    adc_init();
    dma_init();
    adc_buffer[0]=2;
    PMIC.CTRL |= PMIC_LOLVLEN_bm;  // Enable low-level interrupts
    sei();                         // Global interrupt enable
    
    while (1){
        if(cnt==0){
            cnt++;
            PORTD.OUTSET = LED_PIN;  // Set LED
            uart_send_byte(0xAA);
            uart_send_byte(0xBB);
            uart_send_uint16(cnt1++);
            uart_send_uint16(adc_buffer[0]);
            uart_send_uint16(adc_buffer[1]);
            PORTD.OUTCLR = LED_PIN;  // Set LED
        }
    }
}

ISR(TCC1_OVF_vect){
    if(++cnt>3){
        cnt=0;
    }
}
