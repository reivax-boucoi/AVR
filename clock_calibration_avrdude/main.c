#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

// Attiny85 , running @ 8MHZ

// output of 153600 Khz = 9600 baud * 16

// Square wave output on pin 5 (PB0)

// ATMEL ATTINY85 / ARDUINO IDE

int main(void){

// OSCCAL calibrates 8 mhz internal timer speed

// value of 96 was used to get output of 153600 Khz at

// pin 5 on an osciilscope for this particular project


OSCCAL = 0x65; // higher value = faster (see Figure 22-42 in data sheet)
// !!! good value =0x65 pour attiny dip !!!
// !!! good value =0x66 pour attiny sop avec 2pts !!!
// !!! good value =0x6D pour attiny sop avec 1pt !!!



CLKPR = 0x80; // set system clock to 8mhz with no prescale

CLKPR = 0x00; // these 2 CLKPR instructions have to be run together in order

// to set clock to 8 Mhz

DDRB |= (1<<PB0); //Set pin PB0 as output (pinMode(0, OUTPUT)

TCNT0 = 0; // initialize timer counter value to 0

TCCR0A = 0; // write 0 to timer 0 control registers

TCCR0B = 0;

TCCR0A |= (1 << COM0A0); //Timer0 in toggle mode (Table 11-2 of datasheet)

TCCR0A |= (1 << WGM01); //Start timer 1 in CTC mode (Table 11-5 )

TCCR0B |= (1 << CS00); // Prescaler table 11.6

OCR0A=27; //CTC Compare value (27 gives 153 khz)
while(1);
return(0);

}
