#ifndef POWER_H_
#define POWER_H_

#include "periphs.h"

#define NMAX 138 // estimated max 2048, and choose multiples of 40 (/ 46 ??)(TODO check S.R.)
#define NORM (1.0/NMAX) // TODO : choose NORM and NMAX according to 50Hz and F_CPU
#define IMIN 0.001 // TODO check threshold and working

// Flag byte
#define F_FAULT 0x00
#define F_CYCLE_FULL 0x01
#define F_UARTTX 0x02
#define F_UARTRX 0x04
#define F_DEBUG 0x08

volatile struct S_Sample{
	//x[n]		x[n-1]
	int16_t current, previous;
	//y[n]		y[n-1]			z[n]
	int32_t filtered,previousFiltered,calibrated;
}Sample[2]={{0}};

struct S_Acc{
	int32_t v,i,p;
}Acc,Sum;
struct S_Result{
	float v, i , p;
}Res;


volatile struct S_Cal{
	uint8_t phase, gain, zero;
}CalCoeffs[2]={{0,1,0},{0,1,0}};

void acquisition(uint8_t index);
void power_setTimer();

#endif
