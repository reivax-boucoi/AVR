#include <avr/io.h>
#include <avr/interrupt.h>
#include "peripherals.h"
#include "FaderCtrl.h"

volatile uint16_t cnt=1;
volatile int32_t cnt1=0;
volatile uint8_t half = 0;       // tracks which half is being sampled

int main(void){
    clk_init();
    GPIO_init();
    uart_init();
    timer_init();
    PWM_init();
    adc_init();
    dma_init();
    PMIC.CTRL |= PMIC_LOLVLEN_bm;  // Enable low-level interrupts
    sei();                         // Global interrupt enable
    PI_InitAll();
    motTargPos[0]=2048;
    while (1){
        if(cnt==0){
            cnt++;
            //uart_send_byte(0xAA);
            //uart_send_byte(0xBB);
            uart_send_int32_ascii(cnt1++);//uart_send_int32((int32_t)cnt1++);//adc_buffer[0]);
            uart_send_byte('\r');
            uart_send_byte('\n');
        }
    }
}

ISR(TCC1_OVF_vect){
    if(++cnt>500){
        cnt=0;
        PORTD.OUTTGL = LED_PIN;  // Set LED
    }
    PI_UpdateAll();

}
