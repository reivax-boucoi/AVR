#include <avr/io.h>
#include <util/delay.h>
#define READ true
#define WRITE false

#define DATAPORT PORTC
#define DATAREG DDRC

#define ADDPORT1 PORTD
#define ADDPORT2 PORTA

#define WE PINB1
#define OE PINB2
void setMode(bool mode);
void setData(uint16_t a, uint8_t dat);
uint8_t readData(uint16_t a);
int main(void){
	DATAREG = 0x00;
	DATAPORT = 0x00;
	DDRD = 0xFF;
	DDRA = 0xFF;
	//DDRB = 0xFF;
	DDRB |= (1<<PINB0)|(1<<WE)|(1<<OE);
	PORTB &=~(1<<PINB0);
	_delay_ms(1000);
	PORTB |=(1<<PINB0);
	_delay_ms(2000);

	setMode(READ);
	for(uint16_t ad=0;ad<0x1FFF;ad++){
		readData(ad);
	}
	PORTB &=~(1<<PINB0);
	_delay_ms(1000);
	PORTB |=(1<<PINB0);

	setMode(WRITE);
	for(uint16_t ad=0;ad<0x1FFF;ad++){
		//setData(ad,ad&0xFF);
		setData(ad,0x00);
	}
	PORTB &=~(1<<PINB0);
	while(1){
		PORTB^=(1<<PINB0);
		_delay_ms(500);

	}
	return 0;
}

inline void setMode(bool mode) {
	if(mode==READ){
		DATAREG = 0x00;
		DATAPORT = 0xFF;
		PORTB &=~(1<<OE);
		PORTB|=(1<<WE);
	}else if(mode==WRITE){
		PORTB |=(1<<OE)|(1<<WE);
		DATAREG = 0xFF;
	}
	_delay_ms(1);
}

inline void setData(uint16_t a, uint8_t dat) {
	ADDPORT1 = a & 0xFF;
	ADDPORT2 = a & 0x1F00;
	DATAPORT = dat;
	_delay_us(150);
	PORTB &=~(1<<WE);
	_delay_us(150);
	PORTB |=(1<<WE);
	_delay_us(150);
}

inline uint8_t readData(uint16_t a) {
	ADDPORT1 = a & 0xFF;
	ADDPORT2 = a & 0x1F00;
	_delay_ms(500);
	return DATAPORT;
}
