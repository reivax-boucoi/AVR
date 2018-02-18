#ifndef _I2C_H_
#define _I2C_H_ 1


void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_putchar(uint8_t c);
uint8_t i2c_getchar_ack(void);
uint8_t i2c_getchar_nack(void);
void i2c_write(uint8_t addr, uint8_t len, uint8_t adrs, uint8_t *buf);
void i2c_read(uint8_t addr, uint8_t len, uint8_t adrs, uint8_t *buf);
void ERROR(uint8_t err);

#endif