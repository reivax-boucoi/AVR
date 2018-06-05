#include "power.h"

volatile uint8_t scnt = 0; // sample count
volatile uint8_t Flags = 0;

void acquisition(uint8_t index){//reads adc, filters, TODO calibrate and accumulate

	Sample[index].previous = Sample[index].current; // x[n] -> x[n+1]
	// adc read
	switch (index){ // DATA -> x[n]
	case 0:
		Sample[0].current = adc_v();
		break;
	case 1:
		Sample[1].current = adc_i();
		break;
	default:
		Flags|=(1<<F_FAULT);
		break;
	}

	// filtering
	Sample[index].previousFiltered = Sample[index].filtered;  // y[n] -> y[n-1]
	int32_t temp0 = 255*(int32_t)Sample[index].filtered; // =0.996*y[n-1]
	temp0 = temp0>>8;
	int16_t temp1 = Sample[index].current - Sample[index].previous; //=x[n]-x[n-1]
	temp0 = temp0 + ((255*(int32_t)temp1)>>8); // =0.996*(x[n]-x[n-1]) + 0.996*y[n-1]
	Sample[index].filtered = temp0;

	//TODO : Add calibration for phase lag here
	Sample[index].calibrated = Sample[index].filtered;


	// accumulation
	uint32_t temp = (Sample[index].calibrated>>6)*(Sample[index].calibrated>>6); //TODO check shift nbs
	switch (index){
	case 0:
		Acc.v = temp;
		break;
	case 1:
		Acc.i = temp;
		break;
	default:
		Flags|=(1<<F_FAULT);
		break;
	}
}

void power_setTimer() {
	TCCR0A |=(1<<WGM01); // CTC mode
	TIMSK0|=1<<OCIE0A;
	OCR0A=6; // 16MHz/(2*1024*(1+OCR0A))=1.953.125KHz
	TCCR0B |=(1<<CS02) |(1<<CS00); // N=1024
}
