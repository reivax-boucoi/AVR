#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#ifdef I2C_FAST_MODE	
	#define I2C_TLOW	1.3
	#define I2C_THIGH	0.6
#else
	#define I2C_TLOW	4.7
	#define I2C_THIGH	4.0
#endif

#define DDR_USI             DDRB
#define PORT_USI            PORTB
#define PIN_USI             PINB
#define PORT_USI_SDA        PB0
#define PORT_USI_SCL        PB2
#define PIN_USI_SDA         PINB0
#define PIN_USI_SCL         PINB2


enum{
	USI_MASTER_ADDRESS,
	USI_MASTER_WRITE,
	USI_MASTER_READ
} USI_I2C_Master_State;


char USI_I2C_Master_Transfer(char USISR_temp){
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

char USI_I2C_Master_Start_Transmission(char *msg, char msg_size)
{
	USI_I2C_Master_State = USI_MASTER_ADDRESS;

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
			case USI_MASTER_ADDRESS:
				if(!(*msg & 0x01)){
					USI_I2C_Master_State = USI_MASTER_WRITE;
				}else{
					USI_I2C_Master_State = USI_MASTER_READ;
				}//no break so goes to Write
				
			case USI_MASTER_WRITE:
				PORT_USI &= ~(1 << PORT_USI_SCL);
				USIDR = *(msg);				//Load data			
				msg++;						//Increment buffer pointer
				USI_I2C_Master_Transfer(0b11110000 | (0x00<<USICNT0));
				DDR_USI &= ~(1 << PORT_USI_SDA);
				if(USI_I2C_Master_Transfer(0b11110000 | (0x0E<<USICNT0)) & 0x01){
					PORT_USI |=  (1 << PORT_USI_SCL);
					PORT_USI |=  (1 << PORT_USI_SDA);
					return 0;
				}
				DDR_USI |=  (1 << PORT_USI_SDA);
				break;

			case USI_MASTER_READ:
				DDR_USI &= ~(1 << PORT_USI_SDA);
				(*msg) = USI_I2C_Master_Transfer(0b11110000 | (0x00<<USICNT0));			
				msg++;
				DDR_USI |=  (1 << PORT_USI_SDA);				
				if(msg_size == 1){
					USIDR = 0xFF;			//Load NACK to end transmission
				}else{
					USIDR = 0x00;			//Load ACK
				}
				USI_I2C_Master_Transfer(0b11110000 | (0x0E<<USICNT0));
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