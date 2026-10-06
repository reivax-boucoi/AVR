#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "buttons.h"        //Uses TCC1
#include "pwm_sled.h"       //Uses TCD0
#include "pwm_hbridge.h"    //Uses TCC0
#include "gpio_static.h"
#include "uart.h"
#include "adc.h"
#define TOGGLE_DELAY_MS 500
#define TICKER_TIMEOUT (TOGGLE_DELAY_MS/DEBOUNCE_SAMPLE_MS-2)


volatile uint16_t target=0;
volatile int32_t error=0;
volatile uint16_t currentPos=0;
volatile uint16_t ticker=0;
int8_t mduty=1;

static void clock_init(void){
    OSC.CTRL |= OSC_RC32MEN_bm;
    while (!(OSC.STATUS & OSC_RC32MRDY_bm));
    CCP = CCP_IOREG_gc;
    CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
}

ISR(TCC1_OVF_vect){
    buttons_sample();
    target=4096-adc_read_raw(4);
    currentPos=adc_read_raw(3);
    pwm_sled_set_duty(0, (float)target*100.0/4096.0);
    if(mduty>0){
        error=pwm_hbridge_setPos(3,target,currentPos);
    }
    ticker++;
}

int main(void){
    gpio_static_init();
    clock_init();
    adc_init();
    buttons_init();
    timer_init();
    pwm_sled_init();
    pwm_hbridge_init();
    uart_init();
    adc_init();
    sei();
    uart_puts("\r\n\nRestarted\r\n");

    while (1){

        for (uint8_t i = 0; i < NUM_BUTTONS; i++){
            button_state_t st;
            if (buttons_changed(i, &st)){
                if(st == BTN_PRESSED){
                    switch(i){
                        case 0:
                            LED1_ON();
                            LED2_OFF();
                            pwm_hbridge_OFF(3);
                            mduty=0;

                            break;
                        case 1:
                            pwm_hbridge_set_duty(3,0);
                            break;
                        case 2:
                            LED2_ON();
                            LED1_OFF();
                            mduty=1;
                            break;
                    }
                }
            }
        }

        if (ticker>TICKER_TIMEOUT){
            ticker = 0;
            //uint16_t : %u
            // int16_t : %d
            // int32_t : %ld
            printf_P(PSTR("Target = %u\tCurrentPos = %u\tError = %ld\r\n"), target, currentPos, error);

        }
        /*if (uart_rx_available()){
            float v;
            if (scanf("%f", &v) == 1)
                printf_P(PSTR("got %.4f\n"), v);
            else
                puts_P(PSTR("parse error"));
            while (getchar() != '\n');
        }*/
    }
}
