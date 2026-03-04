#ifndef PERIHPERALS_H
#define PERIHPERALS_H
#include <avr/io.h>


#define LED_PIN PIN0_bm   // Bit mask for PIN0
#define F_PWM (24000)
#define F_ADC (1000)

#define PWM_PERIOD ((F_CPU/F_PWM)-1)
#define ADC_PERIOD ((F_CPU/8/F_ADC)-1)
#define UART_BAUD (33) //115200 -> with 1/2 fractional setting (-1:0b1111)

#define XSTR(x) STR(x)
#define STR(x) #x
//#pragma message(XSTR(UART_BAUD))
#if ((ADC_PERIOD > 0x0000FFFF)||(PWM_PERIOD > 0x0000FFFF))
    #error "PERIOD exceeds 16-bit timer limit!"
#endif

#define NUM_MOTORS     4
extern volatile uint16_t adc_buffer[8];
extern volatile uint16_t motTargPos[NUM_MOTORS];

void clk_init(void);
void uart_init(void);
void uart_send_byte(uint8_t data);
void uart_send_int32(int32_t value);
void timer_init(void);
void GPIO_init(void);
void PWM_init(void);
void adc_init(void);
void dma_init(void);

#endif
