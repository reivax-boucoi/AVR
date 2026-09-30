#ifndef GPIO_STATIC_H
#define GPIO_STATIC_H

/* ---- Pin assignment: digital out (static) ---- */
#define LED0_PORT   PORTA
#define LED0_bm     PIN4_bm     /* PA4 */
#define LED1_PORT   PORTA
#define LED1_bm     PIN5_bm     /* PA5 */
#define LED2_PORT   PORTA
#define LED2_bm     PIN6_bm     /* PA6 */
#define LED3_PORT   PORTA
#define LED3_bm     PIN7_bm     /* PA7 */

#define CP_PORT     PORTD
#define CP_bm       PIN4_bm     /* PD4, charge-pump control */
#define NCP_PORT    PORTD
#define NCP_bm      PIN5_bm     /* PD5, charge-pump control (complement) */

#define OE_PORT     PORTF
#define OE_bm       PIN7_bm     /* PF7, active-low H-bridge output enable (shared U4+U5) */


#define LED0_ON()     (LED0_PORT.OUTSET = LED0_bm)
#define LED0_OFF()    (LED0_PORT.OUTCLR = LED0_bm)
#define LED0_TOGGLE() (LED0_PORT.OUTTGL = LED0_bm)

#define LED1_ON()     (LED1_PORT.OUTSET = LED1_bm)
#define LED1_OFF()    (LED1_PORT.OUTCLR = LED1_bm)
#define LED1_TOGGLE() (LED1_PORT.OUTTGL = LED1_bm)

#define LED2_ON()     (LED2_PORT.OUTSET = LED2_bm)
#define LED2_OFF()    (LED2_PORT.OUTCLR = LED2_bm)
#define LED2_TOGGLE()  (LED2_PORT.OUTTGL = LED2_bm)

#define LED3_ON()     (LED3_PORT.OUTSET = LED3_bm)
#define LED3_OFF()    (LED3_PORT.OUTCLR = LED3_bm)
#define LED3_TOGGLE() (LED3_PORT.OUTTGL = LED3_bm)

#define CP_ON()      (CP_PORT.OUTSET = CP_bm)
#define CP_OFF()     (CP_PORT.OUTCLR = CP_bm)
#define NCP_ON()     (NCP_PORT.OUTSET = NCP_bm)
#define NCP_OFF()    (NCP_PORT.OUTCLR = NCP_bm)

#define OE_ON()      (OE_PORT.OUTSET = OE_bm)
#define OE_OFF()     (OE_PORT.OUTCLR = OE_bm)

void gpio_static_init(void);

#endif
