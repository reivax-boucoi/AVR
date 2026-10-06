#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/* Channels: 0..3 = PA0..PA3 (ADCA), 4..7 = PB4..PB7 (ADCB).
 * 12 bit, AREFB reference, free-running sweep + DMA (uses DMA CH0 and CH1). */
#define ADC_NUM_CHANNELS   8

/* IIR low-pass: y += (x - y) / 2^ADC_FILTER_SHIFT. 0 = no filtering.
 * Cutoff ~ fs / (2*pi*2^shift), fs = rate of adc_update() calls. */
#define ADC_FILTER_SHIFT   4

void     adc_init(void);                 /* needs 32 MHz system clock */
void     adc_update(void);               /* run the filter; call at a fixed rate, e.g. 1 kHz */
uint16_t adc_read(uint8_t channel);      /* last filtered value (0..4095), returns immediately */
uint16_t adc_read_raw(uint8_t channel);  /* last unfiltered DMA result, returns immediately */

#endif
