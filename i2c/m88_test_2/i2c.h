#ifndef _I2C_H_
#define _I2C_H_ 1

#define ADC_addr 0x68 //-- a confirmer
#define F_I2C 400000UL

void i2c_init(unsigned long i2c_speed);
uint8_t i2c_start(void);
void i2c_stop(void);
uint8_t i2c_putchar(uint8_t c);
uint8_t i2c_getchar_ack(void);
uint8_t i2c_getchar_nack(void);
uint8_t i2c_write(uint8_t addr, uint8_t len, uint8_t adrs, uint8_t *buf);
uint8_t i2c_read(uint8_t addr, uint8_t len, uint8_t adrs, uint8_t *buf);
