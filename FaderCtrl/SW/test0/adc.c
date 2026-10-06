#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include "adc.h"

static volatile uint16_t raw[ADC_NUM_CHANNELS];   /* written by DMA: [0..3] ADCA, [4..7] ADCB */
static uint32_t acc[ADC_NUM_CHANNELS];            /* filter state, scaled by 2^ADC_FILTER_SHIFT */
static uint16_t out[ADC_NUM_CHANNELS];
static uint8_t  primed;

static uint8_t calib(uint8_t offset){
    uint8_t v;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        NVM.CMD = NVM_CMD_READ_CALIB_ROW_gc;
        v = pgm_read_byte(offset);
        NVM.CMD = NVM_CMD_NO_OPERATION_gc;
    }
    return v;
}

/* Sweep CH0..3 on pins first_pin..first_pin+3, free-running, 12 bit, AREFB.
 * ADC clock = 32 MHz / 64 = 500 kHz -> ~60 us per 4-channel sweep (~17 kHz). */
static void adc_setup(ADC_t *adc, uint8_t cal_ofs, uint8_t first_pin){
    adc->CALL = calib(cal_ofs);
    adc->CALH = calib(cal_ofs + 1);
    adc->CTRLB     = ADC_RESOLUTION_12BIT_gc | ADC_FREERUN_bm;
    adc->REFCTRL   = ADC_REFSEL_AREFB_gc;
    adc->PRESCALER = ADC_PRESCALER_DIV64_gc;
    adc->EVCTRL    = ADC_SWEEP_0123_gc;
    for (uint8_t i = 0; i < 4; i++){
        (&adc->CH0)[i].CTRL    = ADC_CH_INPUTMODE_SINGLEENDED_gc | ADC_CH_GAIN_1X_gc;
        (&adc->CH0)[i].MUXCTRL = (uint8_t)((first_pin + i) << ADC_CH_MUXPOS_gp);
    }
}

/* After each sweep, copy CH0RES..CH3RES (8 bytes) into dst, forever */
static void dma_setup(DMA_CH_t *ch, ADC_t *adc, volatile uint16_t *dst, uint8_t trig){
    uint16_t s = (uint16_t)(uintptr_t)&adc->CH0RES;
    uint16_t d = (uint16_t)(uintptr_t)dst;
    ch->ADDRCTRL = DMA_CH_SRCRELOAD_BURST_gc | DMA_CH_SRCDIR_INC_gc |
    DMA_CH_DESTRELOAD_TRANSACTION_gc | DMA_CH_DESTDIR_INC_gc;
    ch->TRIGSRC  = trig;
    ch->TRFCNT   = 8;
    ch->SRCADDR0 = s;  ch->SRCADDR1 = s >> 8;  ch->SRCADDR2 = 0;
    ch->DESTADDR0 = d; ch->DESTADDR1 = d >> 8; ch->DESTADDR2 = 0;
    ch->CTRLA = DMA_CH_ENABLE_bm | DMA_CH_REPEAT_bm | DMA_CH_BURSTLEN_8BYTE_gc;
}

void adc_init(void){
    /* Inputs, digital buffers off (PA0..3, PB4..7, PB0 = AREFB) */
    PORTA.DIRCLR = 0x0F;
    PORTB.DIRCLR = 0xF1;
    for (uint8_t i = 0; i < 4; i++){
        (&PORTA.PIN0CTRL)[i]     = PORT_ISC_INPUT_DISABLE_gc;
        (&PORTB.PIN4CTRL)[i]     = PORT_ISC_INPUT_DISABLE_gc;
    }
    PORTB.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc;

    adc_setup(&ADCA, 0x20, 0);   /* ADCACAL0/1 in production signature row */
    adc_setup(&ADCB, 0x24, 4);   /* ADCBCAL0/1 */

    DMA.CTRL = DMA_ENABLE_bm;
    dma_setup(&DMA.CH0, &ADCA, &raw[0], DMA_CH_TRIGSRC_ADCA_CH3_gc);
    dma_setup(&DMA.CH1, &ADCB, &raw[4], DMA_CH_TRIGSRC_ADCB_CH3_gc);

    ADCA.CTRLA = ADC_ENABLE_bm | ADC_CH0START_bm;
    ADCB.CTRLA = ADC_ENABLE_bm | ADC_CH0START_bm;

}

uint16_t adc_read_raw(uint8_t ch){
    uint16_t a, b;
    if (ch >= ADC_NUM_CHANNELS) return 0;
    do { a = raw[ch]; b = raw[ch]; } while (a != b);   /* DMA writes the 2 bytes separately */
        return a;
}

void adc_update(void){
    for (uint8_t i = 0; i < ADC_NUM_CHANNELS; i++){
        uint32_t x = adc_read_raw(i);
        if (!primed) acc[i] = x << ADC_FILTER_SHIFT;     /* start at the first sample */
            acc[i] += x - (acc[i] >> ADC_FILTER_SHIFT);
        uint16_t y = (uint16_t)(acc[i] >> ADC_FILTER_SHIFT);
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE){ out[i] = y; }
    }
    primed = 1;
}

uint16_t adc_read(uint8_t ch){
    uint16_t v = 0;
    if (ch >= ADC_NUM_CHANNELS) return 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){ v = primed ? out[ch] : raw[ch]; }
    return v;
}
