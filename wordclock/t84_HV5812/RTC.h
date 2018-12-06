#ifndef RTC_H
#define RTC_H
#include "defines.h"
#include <avr/io.h>
#include <util/delay.h>

struct Ttime {
	uint8_t hour, min, monthDay, month;
    int8_t temp;
}currentTime;
typedef struct Ttime Ttime;

void setCurrentTime(uint8_t hour,uint8_t min,uint8_t monthDay,uint8_t month);
uint8_t minquad(uint8_t min);
uint8_t bcdToDec(uint8_t val);
uint8_t RTC_readTime(struct Ttime *t);
uint8_t RTC_readTemp(void);

uint8_t i2c_write(uint8_t addr, uint8_t adrs, uint8_t val);
uint8_t i2c_read(uint8_t addr, uint8_t len, uint8_t adrs, uint8_t *buf);
enum{
	USI_ADDRESS,
	USI_WRITE,
	USI_READ
} USI_I2C_Master_State;
uint8_t i2c_in_transfer(uint8_t USISR_temp);
uint8_t i2c_in_transmit(uint8_t *msg, uint8_t msg_size);
#endif
