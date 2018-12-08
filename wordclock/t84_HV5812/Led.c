#include "Led.h" 

static const uint8_t ledMap[11]={7,8,10,14,0,4,17,2,12,6,16};


void ledInit(Led* leds){
    DDRA |= CLK|DATA|STROBE|LED;
    PORTA &= ~(CLK|DATA|STROBE|LED);
    
    for(uint8_t i=0;i<NBLEDS;i++){
        leds[i].row=i%2;//leds[0]=0, leds[1]=1
        if(i<10){
            leds[i].col=i;//0-9
        }else{
            leds[i].col=i-9;//1-9
        }
        leds[i].c=BLACK;
    }
}

uint32_t getDataByColor(uint8_t c, uint8_t mode, Led* leds){
    uint32_t data=0x00000000;
    for(uint8_t i=0;i<NBLEDS;i++){
        if(leds[i].row==mode){
            if((leds[i].c & c) ){
                data|=(1<<leds[i].col);
            }
        }
    }
    return data;
}

void ledOn(Led* l,uint8_t c){
    l->c=c;
}
void ledOff(Led* l){
    l->c=BLACK;
}
void clearLeds(Led* l){
    for(uint8_t i=0;i<NBLEDS;i++){
        ledOff(&l[i]);
    }
}

void setLeds(Ttime t,Led* l){
    uint8_t temp=0;
    clearLeds(l);
    ledOn(&l[ILEST],getColor());
    
    temp=currentTime.hour;
    if(currentTime.min>35){
        temp++;
    }
    if(temp)
        temp=temp%12;
    switch(temp){
        case 0 :
            ledOn(&l[MINUIT],getColor());
            break;
        case 12 :
            ledOn(&l[MIDI],getColor());
            break;
        default :
            ledOn(&l[ledMap[(temp-1)]],getColor());
            ledOn(&l[HEURE],getColor());
            break;
    }
    
    temp = minquad(currentTime.min); 
    if(temp > 30){
        ledOn(&l[MOINS],getColor());
        temp=60-temp;
    }
    switch(temp){
        case 10 :
            ledOn(&l[DIX],getColor());
            break;
        case 20 :
            ledOn(&l[VINGT],getColor());
            break;
        case 30 :
            ledOn(&l[ETDEMIE],getColor());
            break;
    }
}

void setLedsNb(int8_t nb, Led* l){
    clearLeds(l);
    if(nb < 0){
        ledOn(&l[MOINS],getColor());
        nb=-nb;
    }
    if(nb>29){
        ledOn(&l[VINGT],getColor());
        ledOn(&l[DIX],getColor());
        nb=nb-30;
    }else if(nb>19){
        ledOn(&l[VINGT],getColor());
        nb=nb-20;
    }else if(nb>11){
        ledOn(&l[DIX],getColor());
        nb=nb-10;
    }
    ledOn(&l[ledMap[nb-1]],getColor());
}
void sendRawData(uint32_t data){
    PORTA &=~(STROBE|CLK);
    for(uint8_t i=0;i<20;i++){
        if((data>>i) & 0x00000001){
            PORTA|=DATA;
        }else{
            PORTA&=~DATA;
        }
        PORTA|=CLK;
        PORTA&=~CLK;
    }
    PORTA|=STROBE;
    PORTA&=~STROBE;
}
