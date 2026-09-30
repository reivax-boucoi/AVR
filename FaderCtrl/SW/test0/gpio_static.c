#include <avr/io.h>
#include "gpio_static.h"

void gpio_static_init(void){
    LED0_PORT.DIRSET = LED0_bm;
    LED1_PORT.DIRSET = LED1_bm;
    LED2_PORT.DIRSET = LED2_bm;
    LED3_PORT.DIRSET = LED3_bm;
    CP_PORT.DIRSET   = CP_bm;
    NCP_PORT.DIRSET  = NCP_bm;
    OE_PORT.DIRSET   = OE_bm;

    LED0_OFF();
    LED1_OFF();
    LED2_OFF();
    LED3_OFF();
    CP_OFF();
    NCP_OFF();
    OE_OFF();
}
