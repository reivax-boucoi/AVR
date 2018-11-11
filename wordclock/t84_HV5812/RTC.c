#include "RTC.h" 

void setCurrentTime(uint8_t hour,uint8_t min,uint8_t monthDay,uint8_t month){
	currentTime.hour=hour;
	currentTime.min=min;
	currentTime.monthDay=monthDay;
	currentTime.month=month;
}

uint8_t minquad(uint8_t min){
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

uint8_t bcdToDec(uint8_t val){
  return ((val/16*10) + (val%16));
}

uint8_t RTC_readTime(struct Ttime *t){
	uint8_t data[7] = {0};
	if(i2c_read(RTC_addr,7,0x00,data)){
		t->hour = bcdToDec(data[2]);
		t->min = bcdToDec(data[1]);
		t->monthDay = bcdToDec(data[4]);
		t->month = bcdToDec(data[5]);
		return 1;
	}else{
		return 0;
	}
}
uint8_t i2c_write(uint8_t addr, uint8_t adrs, uint8_t val){ // only supports 1 byte write
	uint8_t i2c_buffer[3];
	uint8_t i2c_buffer_len = 3;
	i2c_buffer[0] = (addr << 1) | I2CWRITE ;
	i2c_buffer[1] = adrs;
	i2c_buffer[2] = val;
	return i2c_in_transmit(i2c_buffer, i2c_buffer_len);
}
uint8_t i2c_read(uint8_t addr, uint8_t len, uint8_t adrs, uint8_t *buf){
	//set register pointer to adrs
	uint8_t i2c_buffer[len+1];
	uint8_t i2c_buffer_len = 2;
	i2c_buffer[0] = (addr << 1) | I2CWRITE;
	i2c_buffer[1] = adrs;
	i2c_in_transmit(i2c_buffer, i2c_buffer_len);
	//read actual data
	i2c_buffer_len = len+1;
	i2c_buffer[0] = (addr << 1) | I2CREAD;
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