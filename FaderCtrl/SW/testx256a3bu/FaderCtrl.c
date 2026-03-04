#include "FaderCtrl.h"


//extern volatile uint16_t adc_buffer[NUM_MOTORS];


static PI_Controller_t pi[NUM_MOTORS];

static volatile uint16_t* const pwm_reg[NUM_MOTORS] = {
    &TCC0.CCA,
    &TCC0.CCB,
    &TCC0.CCC,
    &TCC0.CCD
};


void PI_InitAll(void){
    for (uint8_t i = 0; i < NUM_MOTORS; i++)    {
        pi[i].integrator = 0;
    }
}



void PI_UpdateAll(void){
    for (uint8_t i = 0; i < NUM_MOTORS; i++){
        int32_t error;
        int32_t prop;
        int32_t control_q15;
        int32_t control;
        int32_t output;

        /* Signed position error */
        error = (int32_t)motTargPos[i] - (int32_t)adc_buffer[i];

        /* Proportional (Q15 domain) */
        prop = KP_Q15 * error;

        /* Integrator update */
        pi[i].integrator += (KI_Q15 * error);

        /* Anti-windup clamp */
        if (pi[i].integrator > INT_MAX_Q15)
            pi[i].integrator = INT_MAX_Q15;
        else if (pi[i].integrator < INT_MIN_Q15)
            pi[i].integrator = INT_MIN_Q15;

        /* PI sum (Q15 domain) */
        control_q15 = prop + pi[i].integrator;

        /* Convert from Q15 */
        control = control_q15 / (int32_t)Q15_SCALE;

        /* Center around 50% duty */
        output = control + (PWM_PERIOD / 2);

        /* Output saturation */
        if (output >= (int32_t)PWM_PERIOD)
            output = (PWM_PERIOD-1);
        else if (output < 1)
            output = 0;
        //uart_send_int32(control);
        //uart_send_int32(output);

        //uart_send_int32(motTargPos[i]);
        /* Write to corresponding PWM channel */
        *pwm_reg[i] = (uint16_t)output;
    }
}
