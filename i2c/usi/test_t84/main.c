#define F_CPU 12000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define AccAddr 0x68
#define READ 0x01
#define WRITE 0x00
#define I2C_FAST_MODE 1

#ifdef I2C_FAST_MODE	 // ~=430kHz
	#define I2C_TLOW	0.8 // 1.3
	#define I2C_THIGH	0.5 // 0.6
#else 
	#define I2C_TLOW	4.7 // ~=100kHz
	#define I2C_THIGH	4.0
#endif

#define DDR_USI             DDRA
#define PORT_USI            PORTA
#define PIN_USI             PINA
#define PORT_USI_SDA        PA6
#define PORT_USI_SCL        PA4
#define PIN_USI_SDA         PINA6
#define PIN_USI_SCL         PINA4


enum{
	USI_ADDRESS,
	USI_WRITE,
	USI_READ
} USI_I2C_Master_State;

char USI_Transfer(char USISR_temp){
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

char USI_Start_Transmission(char *msg, char msg_size){
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
				USI_Transfer(0b11110000 | (0x00<<USICNT0));
				DDR_USI &= ~(1 << PORT_USI_SDA);
				if(USI_Transfer(0b11110000 | (0x0E<<USICNT0)) & 0x01){
					PORT_USI |=  (1 << PORT_USI_SCL);
					PORT_USI |=  (1 << PORT_USI_SDA);
					return 0;
				}
				DDR_USI |=  (1 << PORT_USI_SDA);
				break;

			case USI_READ:
				DDR_USI &= ~(1 << PORT_USI_SDA);
				(*msg) = USI_Transfer(0b11110000 | (0x00<<USICNT0));			
				msg++;
				DDR_USI |=  (1 << PORT_USI_SDA);				
				if(msg_size == 1){
					USIDR = 0xFF;			//Load NACK to end transmission
				}else{
					USIDR = 0x00;			//Load ACK
				}
				USI_Transfer(0b11110000 | (0x0E<<USICNT0));
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

int main(void){
	DDRA|=(1<<PINA0);
	uint8_t i2c_buffer[15];
	int16_t data[7];
	uint8_t i2c_buffer_len = 3;
	//write : 0 , read : 1
	i2c_buffer[0] = (AccAddr << 1) | WRITE ;
	i2c_buffer[1] = 0x6B;
	i2c_buffer[2] = 0x00; // activate
	USI_Start_Transmission(i2c_buffer, i2c_buffer_len);
	_delay_ms(35); // to get stable 1st values
	
	for(;;){
		i2c_buffer_len = 2;		
		i2c_buffer[0] = (AccAddr << 1) | WRITE ; // set reg pointer to 1st bit
		i2c_buffer[1] = 0x3B;
		USI_Start_Transmission(i2c_buffer, i2c_buffer_len);
		
		i2c_buffer_len = 15; // read whole data
		i2c_buffer[0] = (AccAddr << 1) | READ;
		if(USI_Start_Transmission(i2c_buffer, i2c_buffer_len)){
			uint8_t i;
			for(i=0;i<7;i++){
				data[2*i] = (i2c_buffer[2*i+1]<<8 )| i2c_buffer[2*i + 2]; // assemble
			}
			if(data[1]==0){
				PORTA|=(1<<PINA0);
			}else{
				PORTA&=~(1<<PINA0); // led high
			}
		}
		_delay_ms(500);	
	}
}