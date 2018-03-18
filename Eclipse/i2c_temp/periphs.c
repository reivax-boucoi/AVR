#include "periphs.h"


T_uart_buffer uart_buffer_tx, uart_buffer_rx;

void uart_buffer_init(T_uart_buffer *b){
	b->head=0;
	b->tail=0;
}
uint8_t uart_buffer_free(T_uart_buffer *b){
	if(b->head>=b->tail)return (BUFFERSIZE -b->head + b->tail);
	return (b->tail - b->head);
}
uint8_t uart_buffer_length(T_uart_buffer *b){
	if(b->head>=b->tail)return (b->head - b->tail);
	return (BUFFERSIZE +b->head - b->tail);
}
char uart_buffer_readByte(T_uart_buffer *b){
	uint8_t t;
	char res;
	t=b->tail;
	if(uart_buffer_length(b)==0)return 0;
	res=b->data[t++];
	if(t>=BUFFERSIZE)t=0;
	b->tail=t;
	return res;
}
void uart_buffer_writeByte(T_uart_buffer *b,char data){
	uint8_t h;
	h=b->head;
	while(uart_buffer_free(b)<1);//WHAT?
	b->data[h++]=data;
	if(h>=BUFFERSIZE)h=0;
}
uint8_t uart_buffer_read(T_uart_buffer *b, char *data, uint8_t l){
	uint8_t t;
	t=b->tail;
	if(l==0 || uart_buffer_length(b)==0)return 0;
	if(uart_buffer_length(b)<l)l=uart_buffer_length(b);
	while(l--){
		*data++=b->data[t++];
		if(t>=BUFFERSIZE)t=0;
	}
	b->tail = t;
	return 1;
}
uint8_t uart_buffer_write(T_uart_buffer *b,char *data,uint8_t l){
	uint8_t i=0;
	if(l==0)return 0;
	while(uart_buffer_free(b)<l);//WHAT?
	while(l--){
		b->data[i++]=*data++;
		if(i>=BUFFERSIZE)i=0;
	}
	b->head=i;
	return 1;
}

void uart_init (void){
	UBRRH = (BAUDRATE>>8);
	UBRRL = BAUDRATE;
	UCSRB|= (1<<TXEN)|(1<<RXEN)|(1<<RXCIE);
	UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
	uart_buffer_init(&uart_buffer_tx);
	uart_buffer_init(&uart_buffer_rx);
}

void uart_transmit(char data){
	while (!( UCSRA & (1<<UDRE)));
	UDR = data;
}
void uart_transmitNow(char *data){
	while(*data>0){
		uart_transmit(*data++);
	}
}
void uart_transmitBuff(char *data, uint8_t l){
	uart_buffer_write(&uart_buffer_tx,data,l);
	UARTTXEN();
}
unsigned char uart_recieve (void){
	while(!(UCSRA) & (1<<RXC));
	return UDR;
}
uint8_t uart_cnt = 0;

void uart_isr_rx(void){
	uart_buffer_writeByte(&uart_buffer_rx,(char)UDR); // errr...
}
void uart_isr_udre(void){
	if(uart_buffer_length(&uart_buffer_tx)>0){
		UDR=uart_buffer_readByte(&uart_buffer_tx);
	}else{
		UARTTXDIS();;
	}
}

void i2c_init(void){
	TWBR = (uint8_t)TWBR_val;
}
uint8_t i2c_start(uint8_t address){
	TWCR = 0;
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	while( !(TWCR & (1<<TWINT)) );
	if((TWSR & 0xF8) != TW_START) return 1;
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while( !(TWCR & (1<<TWINT)) );
	uint8_t twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;
	return 0;
}
uint8_t i2c_write(uint8_t data){
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while( !(TWCR & (1<<TWINT)) );
	if( (TWSR & 0xF8) != TW_MT_DATA_ACK ) return 1;
	return 0;
}
uint8_t i2c_read_ack(void){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while( !(TWCR & (1<<TWINT)) );
	return TWDR;
}
uint8_t i2c_read_nack(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while( !(TWCR & (1<<TWINT)) );
	return TWDR;
}
void i2c_stop(void){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

volatile uint16_t tempo = 250;
void adc_init(){
	ADMUX|=(1<<ADLAR);
	ADCSRA|=(1<<ADEN)|(1<<ADSC)|(1<<ADATE)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
uint8_t adc_read(void){
	tempo = (ADCH<<2)+2;
	return tempo;
}
