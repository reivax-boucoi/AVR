#include "peripherals.h"

volatile uint16_t adc_buffer[8];

void clk_init(void){
    OSC.CTRL |= OSC_RC32MEN_bm;             // Enable 32 MHz internal oscillator
    while (!(OSC.STATUS & OSC_RC32MRDY_bm));// Wait for it to stabilize
    CCP = CCP_IOREG_gc;                     // Enable change of protected IO registers
    CLK.CTRL = CLK_SCLKSEL_RC32M_gc;        // Switch system clock to 32 MHz
    OSC.PLLCTRL = OSC_PLLSRC_RC32M_gc | 2;  // Configure PLL for 48 MHz (32 MHz × 1.5)
    OSC.CTRL |= OSC_PLLEN_bm;               // Enable PLL
    while (!(OSC.STATUS & OSC_PLLRDY_bm));  // Wait for it to stabilize
}


void timer_init(void){
    TCC1.CTRLA = TC_CLKSEL_DIV8_gc;   // Prescaler 8
    TCC1.PER   = ADC_PERIOD;                  // F_ADC
    TCC1.INTCTRLA = TC_OVFINTLVL_LO_gc;  // Enable low-level overflow interrupt
}


void PWM_init(void){
    // Set pins as outputs
    PORTC.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;

    TCC0.CTRLB = TC_WGMODE_SS_gc |
                 TC0_CCAEN_bm |
                 TC0_CCBEN_bm |
                 TC0_CCCEN_bm |
                 TC0_CCDEN_bm;      // Single-slope PWM

    TCC0.PER = PWM_PERIOD;
    AWEXC.CTRL = AWEX_DTICCAEN_bm |
        AWEX_DTICCBEN_bm |
        AWEX_DTICCCEN_bm |
        AWEX_DTICCDEN_bm;       // Enable pattern generation mode
    //AWEXC.DTBOTH = 0x02; //TODO Deadtime
    AWEXC.OUTOVEN = 0xFF;
        
    TCC0.CTRLA = TC_CLKSEL_DIV1_gc; // Start timer, no prescaler
    TCC0.CCA = 250;
    TCC0.CCB = 500;
    TCC0.CCC = 750;
    TCC0.CCD = 1000;
}


void GPIO_init(void){
    PORTD.DIRSET = LED_PIN;// Set PORTD PIN0 as output    
}

void adc_init(void){
    PORTA.DIRCLR = 0xFF;    // Configure PORTA as inputs
    ADCA.PRESCALER = ADC_PRESCALER_DIV32_gc;
    ADCA.REFCTRL = ADC_REFSEL_INTVCC2_gc;
    ADCA.CTRLB = ADC_RESOLUTION_12BIT_gc;
    
    ADCA.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc | ADC_CH_GAIN_DIV2_gc;
    ADCA.CH1.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc | ADC_CH_GAIN_DIV2_gc;
    ADCA.CH2.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc | ADC_CH_GAIN_DIV2_gc;
    ADCA.CH3.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc | ADC_CH_GAIN_DIV2_gc;
    
    ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
    ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;
    ADCA.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;
    ADCA.CH3.MUXCTRL = ADC_CH_MUXPOS_PIN3_gc;
    
    ADCA.EVCTRL = ADC_SWEEP_0123_gc;
    
    ADCA.CTRLA = ADC_ENABLE_bm;
}

void dma_init(void){
    DMA.CTRL = DMA_ENABLE_bm;

    DMA.CH0.SRCADDR0 = ((uint16_t)&ADCA.CH0.RES) & 0xFF;
    DMA.CH0.SRCADDR1 = (((uint16_t)&ADCA.CH0.RES) >> 8) & 0xFF;
    DMA.CH0.SRCADDR2 = 0;

    DMA.CH0.DESTADDR0 = ((uint16_t)adc_buffer) & 0xFF;
    DMA.CH0.DESTADDR1 = (((uint16_t)adc_buffer) >> 8) & 0xFF;
    DMA.CH0.DESTADDR2 = 0;

    DMA.CH0.TRFCNT = 16;    // 8 samples × 2 bytes
    //DMA.CH0.REPCNT = 4;     // 4 block transfers
    DMA.CH0.CTRLA =
        DMA_CH_BURSTLEN_2BYTE_gc |
        DMA_CH_REPEAT_bm |
        DMA_CH_ENABLE_bm;

    DMA.CH0.ADDRCTRL =
        DMA_CH_SRCRELOAD_NONE_gc |
        DMA_CH_SRCDIR_FIXED_gc |
        DMA_CH_DESTRELOAD_BLOCK_gc |
        DMA_CH_DESTDIR_INC_gc;
    
    DMA.CH0.TRIGSRC = DMA_CH_TRIGSRC_TCC1_OVF_gc;//DMA_CH_TRIGSRC_ADCA_CH0_gc;
}
