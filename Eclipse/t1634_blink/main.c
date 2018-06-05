#include <avr/io.h>
#include <avr/interrupt.h>
uint8_t baregraph(uint8_t v, uint8_t mode){
	uint8_t pre[2][9]={{255,127,63,31,15,7,3,1,0},
			{254,253,251,247,239,223,191,127,255}};

	if(v>227)return pre[mode][0];
	if(v>199)return pre[mode][1];
	if(v>170)return pre[mode][2];
	if(v>142)return pre[mode][3];
	if(v>113)return pre[mode][4];
	if(v>85)return pre[mode][5];
	if(v>56)return pre[mode][6];
	if(v>28)return pre[mode][7];
	return pre[mode][8];
}
volatile uint8_t cnt=0;
volatile uint8_t state=0;
int main(void){
	DDRB|=(1<<PB1)|(1<<PB0);
	DDRA=0xFF;
	PORTA=0xFF;
	PORTB|=(1<<PB0);
	TIMSK|=(1<<TOIE1)|(1<<TOIE0);
	ADMUX|=7;
	ADCSRA|=(1<<ADEN)|(1<<ADIE)|(1<<ADATE);//|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	ADCSRB|=(1<<ADLAR)|(1<<ADTS2);//|(1<<ADTS1);
	sei();
	TCCR1B|=(1<<CS11);
	TCCR0B|=(1<<CS01);
	while(1){
	}
	return 1;
}
ISR(TIM0_OVF_vect){
	//PORTA=baregraph(cnt,0);
	//if((cnt==255) | (cnt==0))state=1-state;
	//cnt=cnt+(state?1:-1);

}
ISR(TIM1_OVF_vect){
	PORTB^=(1<<PB0);
}
ISR(ADC_READY_vect){
	PORTA=baregraph(ADCH,0);//131
	PORTB^=(1<<PB1);
}
