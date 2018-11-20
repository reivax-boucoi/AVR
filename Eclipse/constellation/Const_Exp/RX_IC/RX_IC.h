#include <18F4550.h>
#device adc = 16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES PLL1                     //No PLL PreScaler
#FUSES CPUDIV1                  //No System Clock Postscaler
#FUSES NOUSBDIV                 //USB clock source comes from primary oscillator
#FUSES HS                       //High speed Osc (> 4mhz for PCM/PCH) (>10mhz for PCD)
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOPBADEN                 //PORTB pins are configured as digital I/O on RESET
#FUSES NOLPT1OSC                //Timer1 configured for higher power operation
#FUSES NOSTVREN                 //Stack full/underflow will not cause reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)

#use delay(clock=20000000)

#use rs232(baud=57600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=COM_PORT)
#define GR0 PIN_D0
#define GR1 PIN_D1
#define GG0 PIN_D2
#define GG1 PIN_D3
#define GB0 PIN_D4
#define GB1 PIN_D5
#define WLED PIN_C0


