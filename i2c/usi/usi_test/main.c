#define F_CPU 1000000
#include "usi_master.h"
#define AccAddr 0x40
#define READ 0x01
#define WRITE 0x00

int main(void){
	char i2c_transmit_buffer[3];
	char i2c_transmit_buffer_len = 3;
	//write : 0 , read : 1
	i2c_transmit_buffer[0] = (0x40 << 1) | WRITE ;

	i2c_transmit_buffer[1] = 0x12;  //Register to write or to read

	i2c_transmit_buffer[2] = 0x70;  //Value to write to register

	USI_I2C_Master_Start_Transmission(i2c_transmit_buffer, i2c_transmit_buffer_size);

	for(;;){
		
		
	}
}