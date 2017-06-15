#define F_CPU 18432000UL
#define F_I2C 400000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

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

#define REG_SCL DDRC
#define REG_SDA DDRB
#define PORT_SCL PORTC
#define PORT_SDA PORTB
#define PIN_SCL PINC1
#define PIN_SDA PINB1

struct Ttime {
	uint8_t hour;
	uint8_t min;
	uint8_t minquad;
	uint8_t monthDay;
	uint8_t month;
} currenttime;

uint8_t i2c_in_transfer(uint8_t USISR_temp);
uint8_t i2c_in_transmit(uint8_t *msg, uint8_t msg_size);
uint8_t i2c_write(uint8_t addr, uint8_t adrs, uint8_t val);
uint8_t i2c_read(uint8_t addr, uint8_t len, uint8_t adrs, uint8_t *buf);

void uart_init (void);
void uart_transmit (uint8_t data);
uint8_t uart_recieve (void);
void uart_transmitMult(uint8_t *Data);

uint8_t bcdToDec(uint8_t val);
void RTC_init(void);
uint8_t RTC_readTime(Ttime *t);
uint8_t updateTime(Ttime temp);


char myString[16];

int main(void){
	uart_init();
	uart_transmitMult("Initialized UART\n");
	RTC_init();
	sei(); // should be last setup statement, after inits
	RTC_readTime(currenttime);
	while(1){
		
	}
}

ISR(PCINT0_vect){
	i2c_write(RTC_addr,0x0F,0x08);//reset RTC interrupt flag
	Ttime temp;
	RTC_readTime(temp);
	updateTime(temp);
}

uint8_t updateTime(Ttime temp){
	if(temp.hour !=currenttime.hour || temp.minquad != currenttime.minquad){
		currenttime = temp;
		setLeds(currentcolors,)
		return 1;
	}
	return 0;
}

void RTC_init(void){
	DDRA &=~(1<<PINA6);//set INT pin as input
	PORTA |=(1<<PINA6);
	GIMSK |=(1<<PCIE0);//enable interrupt on pin
	i2c_write(RTC_addr,0x0B,1<<7);//sets A2M2
	i2c_write(RTC_addr,0x0C,1<<7);//sets A2M3
	i2c_write(RTC_addr,0x0D,1<<7);//sets A2M4
	i2c_write(RTC_addr,0x0E,0x1E);//enables RTC interrupt and alarm2
}

uint8_t RTC_readTime(Ttime *t){
	uint8_t data[7] = {0};
	if(i2c_read(RTC_addr,7,0x00,data)){
		t.hour = bcdToDec(data[2]);
		t.min = bcdToDec(data[1]);
		t.monthDay = bcdToDec(data[4] & 0x3F);
		t.month = bcdToDec(data[5]);
		t.minquad = quad(t.min);
		return 1;
	}else{
		return 0;
	}
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

uint8_t i2c_in_transfer(uint8_t USISR_temp){ // i2c feed byte internal function
	USISR = USISR_temp;								//Set USISR as requested by calling function
	do{
		_delay_us(I2C_TLOW);
		USICR = 0b00101011;								//SCL Positive Edge
		while (!(PORT_SCL&(1<<PIN_SCL)));		//Wait for SCL to go high
		_delay_us(I2C_THIGH);
		USICR = 0b00101011;								//SCL Negative Edge
	}while (!(USISR&(1<<USIOIF)));					//Do until transfer is complete
	_delay_us(I2C_TLOW);
	return USIDR;
}

uint8_t i2c_in_transmit(uint8_t *msg, uint8_t msg_size){ // i2c general transmit internal function
	USI_I2C_Master_State = USI_ADDRESS;

	//   Start Condition
	PORT_SCL |=  (1 << PIN_SCL); 						//Setting input makes line pull high
	while (!(PORT_SCL & (1<<PIN_SCL)));		//Wait for SCL to go high

	#ifdef I2C_FAST_MODE
		_delay_us(I2C_THIGH);
	#else
		_delay_us(I2C_TLOW);
	#endif
	REG_SDA |=  (1 << PIN_SDA);
	REG_SCL |=  (1 << PIN_SCL);
	PORT_SDA &= ~(1 << PIN_SDA);
	_delay_us(I2C_THIGH);;
	PORT_SCL &= ~(1 << PIN_SCL);
	_delay_us(I2C_TLOW);
	PORT_SDA |=  (1 << PIN_SDA);
	
	do{
		switch(USI_I2C_Master_State){
			case USI_ADDRESS:
				if(!(*msg & 0x01)){
					USI_I2C_Master_State = USI_WRITE;
				}else{
					USI_I2C_Master_State = USI_READ;
				}//no break so goes to Write
				
			case USI_WRITE:
				PORT_SCL &= ~(1 << PIN_SCL);
				USIDR = *(msg);				//Load data			
				msg++;						//Increment buffer pointer
				i2c_in_transfer(0b11110000 | (0x00<<USICNT0));
				REG_SDA &= ~(1 << PIN_SDA);
				if(i2c_in_transfer(0b11110000 | (0x0E<<USICNT0)) & 0x01){
					PORT_SCL |=  (1 << PIN_SCL);
					PORT_SDA |=  (1 << PIN_SDA);
					return 0;
				}
				REG_SDA |=  (1 << PIN_SDA);
				break;

			case USI_READ:
				REG_SDA &= ~(1 << PIN_SDA);
				(*msg) = i2c_in_transfer(0b11110000 | (0x00<<USICNT0));			
				msg++;
				REG_SDA |=  (1 << PIN_SDA);				
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
	PORT_SDA &= ~(1 << PIN_SDA);           				// Pull SDA low.
	_delay_us(I2C_TLOW);
	REG_SCL &= ~(1 << PIN_SCL);            				// Release SCL.
	while( !(PORT_SCL & (1<<PIN_SCL)) );  	// Wait for SCL to go high.  
	_delay_us(I2C_THIGH);
	REG_SDA &= ~(1 << PIN_SDA);            				// Release SDA.
	while( !(PORT_SCL & (1<<PIN_SDA)) );  	// Wait for SDA to go high. 
	return 1;
}

void uart_init (void){
    UBRR0H = (BAUDRATE>>8);
    UBRR0L = BAUDRATE;	// set baud rate
    UCSR0B|= (1<<TXEN0)|(1<<RXEN0);	// enable receiver and transmitter
    UCSR0C|= (1<<UMSEL01)|(1<<UCSZ00)|(1<<UCSZ01);	// 8bit data format
}

void uart_transmit (uint8_t data){
    while (!( UCSR0A & (1<<UDRE0)));	// wait while register is free
    UDR0 = data;	// load data in the register
}

uint8_t uart_recieve (void){
    while(!(UCSR0A) & (1<<RXC0));	// wait while data is being received
    return UDR0;	// return 8-bit data
}

void uart_transmitMult(uint8_t* data){
	while(*data>0){
		uart_transmit(*data++);
	}
}

uint8_t quad(uint8_t min){
	if(min > 5){
		if(min > 15){
			if(min > 25){
				if(min > 35){
					if(min > 45){
						if(min > 55){
							return 0;
						}else{return 50;}
					}else{return 40;}
				}else{return 30;}
			}else{return 20;}
		}else{return 10;}
	}else{return 0;}
}
