#define F_CPU 16000000
#include <stdio.h>
#include <math.h>
#include <avr/io.h>
//#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 4800
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
#define NMAX 104
#define NORM (1.0/NMAX) // TODO : choose NORM and NMAX according to 50Hz and F_CPU
#define IMIN 0.001 // TODO

// Pin config
#define CS PINB2
#define STATUS PIND5
#define STATUS1 PIND6

// ADC config:xxxxSSx1 0
#define VCH0 0b00001100
#define ICH0 0b00001001 // CH2ref-/3sig+ (diff)

// Flag byte
#define F_FAULT 0x00
#define F_CYCLE_FULL 0x01
#define F_UARTTX 0x02
#define F_UARTRX 0x04
#define F_DEBUG 0x08
a
volatile uint8_t data; //UART buffer
volatile uint8_t scnt = 0; // sample count
volatile uint16_t cnt = 0; // timer extended byte for usart
volatile uint8_t Flags = 0;

volatile uint16_t tcnt = 0;
int16_t testArr[512] = {0};

volatile struct S_Cal{
	uint8_t phase, gain, zero;
}CalCoeffs[2]={{0,1,0},{0,1,0}};

volatile struct S_Sample{
			//x[n]		x[n-1]
	int16_t current, previous;
			//y[n]		y[n-1]			z[n]
	int32_t filtered,previousFiltered,calibrated;
}Sample[2]={{0}};

volatile struct S_Acc{
	int32_t v,i,p;
}Acc,Sum;
struct S_Result{
	float v, i , p;
}Res;

void uart_init (void){
    UBRR0H = (BAUDRATE>>8);
    UBRR0L = BAUDRATE;	// set baud rate
    UCSR0B|= (1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);	// enable receiver and transmitter
    UCSR0C|= (1<<UCSZ00)|(1<<UCSZ01);	// 8bit data format
}
void uart_transmit (uint8_t data){
    while (!( UCSR0A & (1<<UDRE0)));	// wait while register is free
    UDR0 = data;	// load data in the register
}
void uart_transmitMult(char *Data){
	while(*Data>0){
		uart_transmit(*Data++);
	}
}
uint8_t uart_recieve (void){
    while(!(UCSR0A) & (1<<RXC0));	// wait while data is being received
    return UDR0;	// return 8-bit data
}

void spi_masterInit(void){
DDRB = (1<<PINB3)|(1<<PINB5)|(1<<CS);
SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)/*|(1<<SPIE)*/; //sample rising & setup falling
PORTB |=(1<<CS);
}
uint8_t spi_rxtx(uint8_t data){
SPDR = data;
while(!(SPSR & (1<<SPIF)));
return (SPDR);
}


uint16_t adc_v(void){
	uint16_t val=0;
	PORTB &=~(1<<CS);
	spi_rxtx(VCH0);
	val=(spi_rxtx(128) &15)<<8;
	val|=spi_rxtx(0);
	PORTB |=(1<<CS);
	return (val);
}

int16_t adc_i(void){
	uint16_t val=0;
	PORTB &=~(1<<CS);
	spi_rxtx(ICH0);
	uint8_t sign=spi_rxtx(128);;
	val=(sign&15)<<8;
	sign=(sign&0b00010000);
	val|=spi_rxtx(0);
	PORTB |=(1<<CS);
	if(sign)return (val-4096);
	return (val);
}

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
			testArr[tcnt++]=Sample[index].current;
			testArr[tcnt++]=Sample[index].filtered;
		break;
		case 1:
			Acc.i = temp;
		break;
		default:
			Flags|=(1<<F_FAULT);
		break;
	}
}

int main(void){
	// TODO  : Watchdog
	DDRD |=(1<<STATUS)|(1<<STATUS1);
	DDRB |= 1 << PINB0 ;
	PORTB |= 1<<PINB0;
	uart_init();
	uart_transmitMult("\ninitialized uart !\n");
	spi_masterInit();
	uart_transmitMult("initialized spi !\n");
	TCCR0A |=(1<<WGM01); // CTC mode
	TIMSK0|=1<<OCIE0A;
	OCR0A=6; // 16MHz/(2*1024*(1+OCR0A))=1.953.125KHz
	sei();
	TCCR0B |=(1<<CS02) |(1<<CS00); // N=1024

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
