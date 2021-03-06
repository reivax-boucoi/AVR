#define F_CPU 16000000UL
#define F_I2C 400000UL

#include <avr/io.h>
#include <util/delay.h>

#define BAUD 9600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

#define RTC_addr 0x68

#define READ 0x01
#define WRITE 0x00
#define I2C_FAST_MODE 1

#ifdef I2C_FAST_MODE	 // ~=430kHz
	#define I2C_TLOW	1 // 1.3
	#define I2C_THIGH	0.7 // 0.6
#else 
	#define I2C_TLOW	4.7
	#define I2C_THIGH	4.0
#endif

#define DDR_USI             DDRA
#define PORT_USI            PORTA
#define PIN_USI             PINA
#define PORT_USI_SDA        PA6
#define PORT_USI_SCL        PA4
#define PIN_USI_SDA         PINA6
#define PIN_USI_SCL         PINA4

 struct Ttime {
	uint8_t hour, min, minquad, monthDay, month;
};
struct Ttime currenttime;
 struct Tcolors {
	uint8_t r,g,b;
};
struct Tcolors currentcolors;

uint8_t i2c_in_transfer(uint8_t USISR_temp);
uint8_t i2c_in_transmit(uint8_t *msg, uint8_t msg_size);
uint8_t i2c_write(uint8_t addr, uint8_t adrs, uint8_t val);
uint8_t i2c_read(uint8_t addr, uint8_t len, uint8_t adrs, uint8_t *buf);

uint8_t bcdToDec(uint8_t val);
void RTC_init(void);
uint8_t RTC_readTime(struct Ttime *t);
uint8_t updateTime(struct Ttime temp);
uint8_t quad(uint8_t min);
void setLeds(struct Tcolors c, struct Ttime t);

uint8_t myString[16];
uint8_t data[7] = {0};

int main(void){
	RTC_init();
	DDRA |= (1<<PINA0);
	PORTB &=~(1<<PINA0);
	while(1){
		struct Ttime temp;
		RTC_readTime(&temp);
		updateTime(temp);
		_delay_ms(500);	
		}
}
/*
ISR(_vect){//-------------------------------------------------------------------------------------------------------------------------------
	i2c_write(RTC_addr,0x0F,0x08);//reset RTC interrupt flag
	struct Ttime temp;
	RTC_readTime(temp);
	updateTime(temp);
}
*/
void setLeds(struct Tcolors c, struct Ttime t){
	PORTA ^=(1<<PINA0);
}
uint8_t updateTime(struct Ttime temp){
	if(temp.minquad != currenttime.minquad ||
		temp.hour != currenttime.hour){
		currenttime = temp;
		setLeds(currentcolors,currenttime);
		return 1;
	}
	return 0;
}
void RTC_init(void){
	//set INT pin as input--------------------------------------------------------------------------------------------------------------
	//enable interrupt on pin
	//i2c_write(RTC_addr,0x0B,1<<7);//sets A2M2
	//i2c_write(RTC_addr,0x0C,1<<7);//sets A2M3
	//i2c_write(RTC_addr,0x0D,1<<7);//sets A2M4
	//i2c_write(RTC_addr,0x0E,0x1E);//enables RTC interrupt and alarm2
}
uint8_t RTC_readTime(struct Ttime *t){
	uint8_t data[7] = {0};
	if(i2c_read(RTC_addr,7,0x00,data)){
		t->hour = bcdToDec(data[2]);
		t->min = bcdToDec(data[1]);
		t->monthDay = bcdToDec(data[4]);
		t->month = bcdToDec(data[5]);
		t->minquad = quad(t->min);
		return 1;
	}else{
		return 0;
	}
}

uint8_t quad(uint8_t min){
	if(min >= 5){
		if(min >= 15){
			if(min >= 25){
				if(min >= 35){
					if(min >= 45){
						if(min >= 55){
							return 0;
						}else{return 50;}
					}else{return 40;}
				}else{return 30;}
			}else{return 20;}
		}else{return 10;}
	}else{return 0;}
}
uint8_t bcdToDec(uint8_t val){
  return ((val/16*10) + (val%16));
}

uint8_t i2c_write(uint8_t addr, uint8_t adrs, uint8_t val){ // only supports 1 byte write
	uint8_t i2c_buffer[3];
	uint8_t i2c_buffer_len = 3;
	i2c_buffer[0] = (addr << 1) | WRITE ;
	i2c_buffer[1] = adrs;
	i2c_buffer[2] = val;
	return i2c_in_transmit(i2c_buffer, i2c_buffer_len);
}
uint8_t i2c_read(uint8_t addr, uint8_t len, uint8_t adrs, uint8_t *buf){
	//set register pointer to adrs
	uint8_t i2c_buffer[len+1];
	uint8_t i2c_buffer_len = 2;
	i2c_buffer[0] = (addr << 1) | WRITE;
	i2c_buffer[1] = adrs;
	i2c_in_transmit(i2c_buffer, i2c_buffer_len);
	//read actual data
	i2c_buffer_len = len+1;
	i2c_buffer[0] = (addr << 1) | READ;
	if(i2c_in_transmit(i2c_buffer, i2c_buffer_len)){
		uint8_t i;
		for(i=1;i<len+1;i++){ // fetch in buffer
			buf[i-1]=i2c_buffer[i];
		}
		return 1; // provide success check
	}else{
		return 0;
	}
}
enum{
	USI_ADDRESS,
	USI_WRITE,
	USI_READ
} USI_I2C_Master_State;
uint8_t i2c_in_transfer(uint8_t USISR_temp){
	USISR = USISR_temp;								//Set USISR as requested by calling function
	do{
		_delay_us(I2C_TLOW);
		USICR = 0b00101011;								//SCL Positive Edge
		while (!(PIN_USI&(1<<PIN_USI_SCL)));		//Wait for SCL to go high
		_delay_us(I2C_THIGH);
		USICR = 0b00101011;								//SCL Negative Edge
	}while (!(USISR&(1<<USIOIF)));					//Do until transfer is complete
	_delay_us(I2C_TLOW);
	return USIDR;
}
uint8_t i2c_in_transmit(uint8_t *msg, uint8_t msg_size){
	USI_I2C_Master_State = USI_ADDRESS;

	//   Start Condition
	PORT_USI |=  (1 << PORT_USI_SCL); 						//Setting input makes line pull high
	while (!(PIN_USI & (1<<PIN_USI_SCL)));		//Wait for SCL to go high

	#ifdef I2C_FAST_MODE
		_delay_us(I2C_THIGH);
	#else
		_delay_us(I2C_TLOW);
	#endif
	DDR_USI |=  (1 << PORT_USI_SDA);
	DDR_USI |=  (1 << PORT_USI_SCL);
	PORT_USI &= ~(1 << PORT_USI_SDA);
	_delay_us(I2C_THIGH);;
	PORT_USI &= ~(1 << PORT_USI_SCL);
	_delay_us(I2C_TLOW);
	PORT_USI |=  (1 << PORT_USI_SDA);
	
	do{
		switch(USI_I2C_Master_State){
			case USI_ADDRESS:
				if(!(*msg & 0x01)){
					USI_I2C_Master_State = USI_WRITE;
				}else{
					USI_I2C_Master_State = USI_READ;
				}//no break so goes to Write
				
			case USI_WRITE:
				PORT_USI &= ~(1 << PORT_USI_SCL);
				USIDR = *(msg);				//Load data			
				msg++;						//Increment buffer pointer
				i2c_in_transfer(0b11110000 | (0x00<<USICNT0));
				DDR_USI &= ~(1 << PORT_USI_SDA);
				if(i2c_in_transfer(0b11110000 | (0x0E<<USICNT0)) & 0x01){
					PORT_USI |=  (1 << PORT_USI_SCL);
					PORT_USI |=  (1 << PORT_USI_SDA);
					return 0;
				}
				DDR_USI |=  (1 << PORT_USI_SDA);
				break;

			case USI_READ:
				DDR_USI &= ~(1 << PORT_USI_SDA);
				(*msg) = i2c_in_transfer(0b11110000 | (0x00<<USICNT0));			
				msg++;
				DDR_USI |=  (1 << PORT_USI_SDA);				
				if(msg_size == 1){
					USIDR = 0xFF;			//Load NACK to end transmission
				}else{
					USIDR = 0x00;			//Load ACK
				}
				i2c_in_transfer(0b11110000 | (0x0E<<USICNT0));
				break;
		}
	}while(--msg_size);
	
	//Stop Condition
	PORT_USI &= ~(1 << PORT_USI_SDA);           				// Pull SDA low.
	_delay_us(I2C_TLOW);
	DDR_USI &= ~(1 << PORT_USI_SCL);            				// Release SCL.
	while( !(PIN_USI & (1<<PIN_USI_SCL)) );  	// Wait for SCL to go high.  
	_delay_us(I2C_THIGH);
	DDR_USI &= ~(1 << PORT_USI_SDA);            				// Release SDA.
	while( !(PIN_USI & (1<<PIN_USI_SDA)) );  	// Wait for SDA to go high. 
	return 1;
}
