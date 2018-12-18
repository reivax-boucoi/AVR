#include <avr/io.h>
#include <util/delay.h>

uint32_t integerRoot(uint32_t x){
    if(x<2) return x;
    uint32_t res=0;
    uint32_t add= 0x8000;
    for(int i=0;i<16;i++)    {
        uint16_t temp=res | add;
        uint32_t g2=(temp-1)*(temp-1);
        if (x>g2){
            res=temp;
        }
        add>>=1;
    }
    return res;
}
bool isPrime(uint32_t i){
	uint16_t max=integerRoot(i);
	for(uint32_t j=3;j<max;j+=2){
		if(i%j==0)return false;
	}
	return true;
}
int main(void){
	DDRB|=(1<<PB4);
	uint32_t i = 4;
	while(1){
		_delay_ms(500);
		if(isPrime(i))PORTB^=(1<<PB4);
		if(i>999983){
			while(1);
		}
	}
	return 0;
}

