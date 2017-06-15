#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

int main(void){
  DDRB |= 1<<PINB0;
  uint8_t myByte;
  myByte = eeprom_read_byte((uint8_t*)46);
  eeprom_update_byte((uint8_t*)46,myByte);
  
  while(1){
	PORTB |= 1<<PINB0;
	_delay_ms(5);
	PORTB &= ~(1<<PINB0);
	_delay_ms(2000);
	
  }
  return 0;
}