#include <avr/io.h>
#include <util/delay.h>
#define READ true
#define WRITE false

#define DATAPORT PORTC
#define DATAREG DDRC

#define ADDPORT1 PORTB
#define ADDPORT2 PORTA

#define WE PIND5
#define OE PIND6
void setMode(bool mode);
void setData(uint16_t a, uint8_t dat);
uint8_t readData(uint16_t a);
int main(void){
	DATAREG = 0x00;
	DATAPORT = 0x00;
	DDRB = 0xFF;
	DDRA = 0xFF;
	DDRD |= (1<<PIND7)|(1<<WE)|(1<<OE);
	PORTD &=~(1<<PIND7);
	_delay_ms(1000);
	PORTD |=(1<<PIND7);
	_delay_ms(2000);

	setMode(READ);
	for(uint16_t ad=0;ad<0x1FFF;ad++){
		readData(ad);
	}
	PORTD &=~(1<<PIND7);
	_delay_ms(1000);
	PORTD |=(1<<PIND7);

	setMode(WRITE);
	for(uint16_t ad=0;ad<0x1FFF;ad++){
		//setData(ad,ad&0xFF);
		setData(ad,0x00);
	}
	PORTD &=~(1<<PIND7);
	while(1){
		PORTD^=(1<<PIND7);
		_delay_ms(500);

	}
	return 0;
}

inline void setMode(bool mode) {
	if(mode==READ){
		DATAREG = 0x00;
		DATAPORT = 0xFF;
		PORTD &=~(1<<OE);
		PORTD|=(1<<WE);
	}else if(mode==WRITE){
		PORTD |=(1<<OE)|(1<<WE);
		DATAREG = 0xFF;
	}
	_delay_ms(1);
}

inline void setData(uint16_t a, uint8_t dat) {
	ADDPORT1 = a & 0xFF;
	ADDPORT2 = a & 0x1F00;
	DATAPORT = dat;
	_delay_us(100);
	PORTD &=~(1<<WE);
	_delay_us(100);
	PORTD |=(1<<WE);
	_delay_us(100);
}

inline uint8_t readData(uint16_t a) {
	ADDPORT1 = a & 0xFF;
	ADDPORT2 = a & 0x1F00;
	_delay_us(1);
	return DATAPORT;
}
