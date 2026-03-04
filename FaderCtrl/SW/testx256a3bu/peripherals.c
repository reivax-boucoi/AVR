#include "peripherals.h"

volatile uint16_t adc_buffer[8];
volatile uint16_t motTargPos[NUM_MOTORS];

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
    
    EVSYS.CH0MUX=0b11001000;//generate event on channel 0 for timer C1 overflow
}


void uart_init(void){
    PORTF.DIRSET = PIN3_bm; //PF3 / TX output
    PORTF.DIRCLR = PIN2_bm; // PF2 / RX input

    USARTF0.CTRLB = 0;//USART_CLK2X_bm;//double uart speed

    USARTF0.BAUDCTRLA = UART_BAUD & 0xFF;   //Baud: BSEL = 6, BSCALE = 0
    USARTF0.BAUDCTRLB = (0b1111<<4) | (UART_BAUD >> 8);

    USARTF0.CTRLC = USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc;
    USARTF0.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;
}

void uart_send_byte(uint8_t data){
    while (!(USARTF0.STATUS & USART_DREIF_bm));
    USARTF0.DATA = data;
}
void uart_send_int32(int32_t value){
    uart_send_byte(value >> 24);
    uart_send_byte((value >> 16)&0xFF);
    uart_send_byte((value >> 8)&0xFF);
    uart_send_byte(value & 0xFF);
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
    ADCA.REFCTRL = ADC_REFSEL_AREFB_gc;
    ADCA.CTRLB = ADC_RESOLUTION_12BIT_gc;
    
    ADCA.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc | ADC_CH_GAIN_1X_gc;
    ADCA.CH1.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc | ADC_CH_GAIN_1X_gc;
    ADCA.CH2.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc | ADC_CH_GAIN_1X_gc;
    ADCA.CH3.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc | ADC_CH_GAIN_1X_gc;
    
    ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
    ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;
    ADCA.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;
    ADCA.CH3.MUXCTRL = ADC_CH_MUXPOS_PIN3_gc;
    
    ADCA.EVCTRL = ADC_SWEEP_0123_gc | ADC_EVACT_SWEEP_gc | ADC_EVSEL_0123_gc; //Event 0 triggers sweep of channels 0-3
    
    ADCA.CTRLA = ADC_DMASEL_CH0123_gc | ADC_ENABLE_bm;
}

void dma_init(void){

    DMA.CH0.CTRLA = DMA_CH_ENABLE_bm | DMA_CH_REPEAT_bm | DMA_CH_BURSTLEN_2BYTE_gc;
    DMA.CH1.CTRLA = DMA_CH_ENABLE_bm | DMA_CH_REPEAT_bm | DMA_CH_BURSTLEN_2BYTE_gc;
    DMA.CH2.CTRLA = DMA_CH_ENABLE_bm | DMA_CH_REPEAT_bm | DMA_CH_BURSTLEN_2BYTE_gc;
    DMA.CH3.CTRLA = DMA_CH_ENABLE_bm | DMA_CH_REPEAT_bm | DMA_CH_BURSTLEN_2BYTE_gc;

    DMA.CH0.TRIGSRC = DMA_CH_TRIGSRC_ADCA_CH0_gc;
    DMA.CH1.TRIGSRC = DMA_CH_TRIGSRC_ADCA_CH0_gc;
    DMA.CH2.TRIGSRC = DMA_CH_TRIGSRC_ADCA_CH2_gc;
    DMA.CH3.TRIGSRC = DMA_CH_TRIGSRC_ADCA_CH3_gc;
    
    DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_BLOCK_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTRELOAD_BLOCK_gc | DMA_CH_DESTDIR_INC_gc;
    DMA.CH1.ADDRCTRL = DMA_CH_SRCRELOAD_BLOCK_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTRELOAD_BLOCK_gc | DMA_CH_DESTDIR_INC_gc;
    DMA.CH2.ADDRCTRL = DMA_CH_SRCRELOAD_BLOCK_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTRELOAD_BLOCK_gc | DMA_CH_DESTDIR_INC_gc;
    DMA.CH3.ADDRCTRL = DMA_CH_SRCRELOAD_BLOCK_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTRELOAD_BLOCK_gc | DMA_CH_DESTDIR_INC_gc;
    
    DMA.CH0.TRFCNT = 2;
    DMA.CH1.TRFCNT = 2;
    DMA.CH2.TRFCNT = 2;
    DMA.CH3.TRFCNT = 2;

    DMA.CH0.SRCADDR0 = ((uint16_t)&ADCA.CH0.RES) & 0xFF;
    DMA.CH0.SRCADDR1 = (((uint16_t)&ADCA.CH0.RES) >> 8) & 0xFF;
    DMA.CH0.DESTADDR0 = ((uint16_t)&adc_buffer[0]) & 0xFF;
    DMA.CH0.DESTADDR1 = (((uint16_t)&adc_buffer[0]) >> 8) & 0xFF;
    
    DMA.CH1.SRCADDR0 = ((uint16_t)&ADCA.CH1.RES) & 0xFF;
    DMA.CH1.SRCADDR1 = (((uint16_t)&ADCA.CH1.RES) >> 8) & 0xFF;
    DMA.CH1.DESTADDR0 = ((uint16_t)&adc_buffer[1]) & 0xFF;
    DMA.CH1.DESTADDR1 = (((uint16_t)&adc_buffer[1]) >> 8) & 0xFF;
    
    DMA.CH2.SRCADDR0 = ((uint16_t)&ADCA.CH2.RES) & 0xFF;
    DMA.CH2.SRCADDR1 = (((uint16_t)&ADCA.CH2.RES) >> 8) & 0xFF;
    DMA.CH2.DESTADDR0 = ((uint16_t)&adc_buffer[2]) & 0xFF;
    DMA.CH2.DESTADDR1 = (((uint16_t)&adc_buffer[2]) >> 8) & 0xFF;
    
    DMA.CH3.SRCADDR0 = ((uint16_t)&ADCA.CH3.RES) & 0xFF;
    DMA.CH3.SRCADDR1 = (((uint16_t)&ADCA.CH3.RES) >> 8) & 0xFF;
    DMA.CH3.DESTADDR0 = ((uint16_t)&adc_buffer[3]) & 0xFF;
    DMA.CH3.DESTADDR1 = (((uint16_t)&adc_buffer[3]) >> 8) & 0xFF;
    
    DMA.CTRL = DMA_ENABLE_bm;
}
