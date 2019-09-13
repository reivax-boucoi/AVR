#ifndef I2C_H
#define I2C_H

#include <avr/io.h>
#include <util/delay.h>
#include <compat/twi.h>

#define I2C_START 0
#define I2C_DATA  1
#define I2C_STOP  2
#define MAX_TRIES 50

unsigned char i2c_transmit(unsigned char type) ;

int i2c_writebyte(unsigned int i2c_address, unsigned int dev_id,unsigned int dev_addr,char data) ;

int i2c_readbyte(unsigned int i2c_address, unsigned int dev_id,unsigned int dev_addr, char *data);


#endif
