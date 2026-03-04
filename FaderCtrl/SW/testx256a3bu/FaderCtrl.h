#ifndef FADER_CTRL_H_
#define FADER_CTRL_H_

#include <avr/io.h>
#include "peripherals.h"

#define MOT_R          (17.0f)        // Ohm
#define MOT_L          (0.0065f)      // Henry
#define MOT_VDRIVE     (7.0f)         // Volt



#define TE             (1.0f / F_ADC)
#define KP_FLOAT       (MOT_L / (MOT_VDRIVE * TE))
#define KI_FLOAT       0//(MOT_R / MOT_VDRIVE)             // L/R
#define Q15_SCALE      32768.0f
#define KP_Q15         ((int32_t)(KP_FLOAT * Q15_SCALE))
#define KI_Q15         ((int32_t)(KI_FLOAT * Q15_SCALE))
#define INT_MAX_Q15    ((int32_t)PWM_PERIOD * (int32_t)Q15_SCALE)
#define INT_MIN_Q15    (-(INT_MAX_Q15))


typedef struct{
    int32_t integrator;     // Q15 scaled
} PI_Controller_t;

void PI_InitAll(void);
void PI_UpdateAll(void);

#endif
