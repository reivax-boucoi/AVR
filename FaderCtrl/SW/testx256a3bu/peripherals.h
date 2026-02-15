#ifndef PERIHPERALS_H
#define PERIHPERALS_H
#include <avr/io.h>


#define LED_PIN PIN0_bm   // Bit mask for PIN0
#define F_PWM (24000UL)
#define PWM_PERIOD ((F_CPU/F_PWM)-1)
#define F_ADC (1000)
#define ADC_PERIOD ((F_CPU/8/F_ADC)-1)

#if ((ADC_PERIOD > 0x0000FFFF)||(PWM_PERIOD > 0x0000FFFF))
    #error "ADC_PERIOD exceeds 16-bit timer limit!"
#endif

extern volatile uint16_t adc_buffer[8];

void clk_init(void);
void timer_init(void);
void GPIO_init(void);
void PWM_init(void);
void adc_init(void);
void dma_init(void);

#endif
