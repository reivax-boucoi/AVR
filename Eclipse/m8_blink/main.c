#include <avr/io.h>
#include <util/delay.h>

int main(void){
	DDRC|=(1<<PC1)|(1<<PC0);
	PORTC^=(1<<PC1);
	while(1){
		PORTC^=(1<<PC1)|(1<<PC0);
		_delay_ms(500);
	}
	return 1;
}
