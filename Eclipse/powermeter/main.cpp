#define F_CPU 16000000
#include <stdio.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "periphs.h"
#include "power.h"


// Pin config
#define STATUS PIND5
#define STATUS1 PIND6


volatile uint8_t data; //UART buffer
volatile uint16_t cnt = 0; // timer extended byte for usart

volatile uint16_t tcnt = 0;
int16_t testArr[512] = {0};



int main(void){
	// TODO  : Watchdog
	DDRD |=(1<<STATUS)|(1<<STATUS1);
	DDRB |= 1 << PINB0 ;
	PORTB |= 1<<PINB0;
	uart_init();
	uart_transmitMult("\ninitialized uart !\n");
	spi_masterInit();
	uart_transmitMult("initialized spi !\n");
	power_setTimer();
	sei();

	while(1){
		if(Flags&F_FAULT){
			uart_transmitMult("FAULT\n");
		}
		if(Flags&F_CYCLE_FULL){
			Flags=Flags&(0xFF-F_CYCLE_FULL);
			Sum = Acc;
			Acc.v=0;
			Acc.i=0;
			Acc.p=0;
			uint16_t temp0;
			temp0 = Sum.v*NORM;
			Res.v = sqrt(temp0)*CalCoeffs[0].gain;
			temp0 = Sum.i*NORM;
			Res.i = sqrt(temp0)*CalCoeffs[1].gain;
			if(Res.i<IMIN){
				Res.p=0.0;
			}else{
				Res.p = Sum.p*NORM*CalCoeffs[0].gain*CalCoeffs[1].gain;
			}
		}
		if(Flags&F_UARTRX){//TODO : add user input cal here
			Flags=Flags&(0xFF-F_UARTRX);
			uart_transmit(data+1);
			if(data=='a'){
				PORTD ^=(1<<STATUS1);
				Flags|=F_DEBUG;
			}
			data=0;
		}
		if(Flags&F_UARTTX){
			Flags=Flags&(0xFF-F_UARTTX);
			PORTD |=(1<<STATUS); // debug
			if(Flags&F_DEBUG){
				cli();
				// TODO : stream results better
				for(uint16_t i = 0;i<256;i++){
					char str[40] = {0};
					//sprintf(str, "%04.2lf\r\n",4.56);
					sprintf(str,"%d,%d\r\n",testArr[2*i],testArr[2*i+1]);
					uart_transmitMult(str);
				}
				sei();
			}
			PORTD &=~(1<<STATUS); // debug
		}
	}
	return 0;
}

ISR(USART_RX_vect, ISR_BLOCK){
	data=uart_recieve();
	Flags|=F_UARTRX;
}
ISR(TIMER0_COMPA_vect){
	PORTD |=(1<<STATUS1); // debug
	acquisition(0);
	acquisition(1);
	Acc.p += (Sample[0].calibrated>>6)*(Sample[1].calibrated>>6); // v*i
	if(tcnt>508/*++scnt>NMAX*/){
		tcnt = 0;
		scnt=0;
		Flags|=F_CYCLE_FULL;
	}
	cnt++;
	if(cnt>=2048){ // TODO pick appropriately
		Flags|=F_UARTTX;
		cnt=0;
	}

	PORTD &=~(1<<STATUS1); // debug
}
