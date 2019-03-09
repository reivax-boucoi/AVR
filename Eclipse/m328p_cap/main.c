#include <avr/io.h>
#include <avr/interrupt.h>


#define BAUD 19200
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

#define BUFFLEN 64

uint8_t buff[BUFFLEN]={};
uint8_t head=0;
uint8_t tail=0;

void flush(void){
	tail=0;
	head=0;
}
uint8_t add(uint8_t data){
	buff[head]=data;
	head=(head+1)%BUFFLEN;
	return (head==tail);
}
uint8_t read(void){
	uint8_t data=buff[tail];
	tail=(tail+1)%BUFFLEN;
	return data;
}
uint8_t isAvailable(void){
	return head!=tail;
}

void uart_transmit(uint8_t data){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

int main(void){
	DDRB|=(1<<PB0);
	DDRD|=(1<<PD6);

	UBRR0H = (BAUDRATE>>8);
	UBRR0L = BAUDRATE;
	UCSR0B = ((1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0));
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);

	flush();
	add(118);
	add(94);
	add(79);
	add(70);
	add(59);
	add(70);
	add(79);
	add(94);
	add(118);

	TCCR0A|=(1<<COM0A0)|(1<<WGM01);
	TCCR1B|=(1<<WGM12);
	OCR0A=0;
	OCR1A=16384;
	TIMSK1|=(1<<OCIE1A);

	sei();
	TCCR0B|=(1<<CS02);//|(1<<CS00);//256
	TCCR1B|=(1<<CS12)|(1<<CS10);
	while(1){

	}
	return 1;
}
ISR(TIMER1_COMPA_vect){
	PORTB^=(1<<PB0);
	if(isAvailable()){
		OCR0A=read();
		uart_transmit('C');
	}else{
		OCR0A=0;
		uart_transmit('A');
	}
}
ISR(USART_RXC_vect){
	//OCR0A=UDR0;
	uart_transmit('B');
}
