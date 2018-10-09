#include "Led.h" 

static const uint8_t ledMap[11]={7,8,10,14,0,4,17,2,12,6,16};

Tcolor tcolor(uint8_t r,uint8_t g,uint8_t b){
    Tcolor c={r,g,b};
    return c;
}

Tcolor tcolorV(uint32_t v){
    Tcolor c={(v>>16)&0xFF,(v>>8)&0xFF,v&0xFF};
    return c;
}

void ledInit(Led* leds){
    for(uint8_t i=0;i<NBLEDS;i++){
        leds[i].row=i%2;//leds[0]=0, leds[1]=1
        if(i<10){
            leds[i].col=i;//0-9
        }else{
            leds[i].col=i-9;//1-9
        }
        leds[i].c.r=0;
        leds[i].c.g=0;
        leds[i].c.b=0;
    }
    leds[1].c.g=1;
}
uint32_t getColorByLed(Led l){
    uint32_t d=0;
    if(l.row){
        if(l.c.r)d|=R2;
        if(l.c.g)d|=G2;
        if(l.c.b)d|=B2;
    }else{
        if(l.c.r)d|=R1;
        if(l.c.g)d|=G1;
        if(l.c.b)d|=B1;
    }
    return d;
}

uint32_t getDataByColor(Tcolor c, uint8_t mode, Led* leds){
    uint32_t data=0;
    for(uint8_t i=0;i<NBLEDS;i++){
        if(leds[i].row==mode){
            if((leds[i].c.r && c.r) || (leds[i].c.g && c.g) || (leds[i].c.b && c.b)){
                data|=(1<<leds[i].col);
            }
        }
    }
    return data;
}

void ledOff(Led* l){
    l->c.r=0;
    l->c.g=0;
    l->c.b=0;
}
void ledOn(Led* l){
    l->c.r=255;
    l->c.g=255;
    l->c.b=255;
}

void ledOnC(Led* l,Tcolor c){
    l->c=c;   
}

void ledOnV(Led* l,uint32_t v){
    l->c=tcolorV(v);
}

uint8_t ledIsOff(Led l){
    return !ledIsOn(l);
}
uint8_t ledIsOn(Led l){
    return l.c.r || l.c.g || l.c.b;
}
void setLeds(Ttime t,Led* l,Tcolor c){
    uint8_t i=0;
    for(;i<NBLEDS;i++){
        ledOff(&l[i]);
    }
    ledOnC(&l[ILEST],c);
    
    switch(currentTime.hour){
        case 0 :
            ledOnC(&l[MINUIT],c);
            break;
        case 12 :
            ledOnC(&l[MIDI],c);
            break;
        default :
            if(currentTime.min>35){
                ledOnC(&l[ledMap[(currentTime.hour%12)]],c);
            }else{
                ledOnC(&l[ledMap[(currentTime.hour%12)-1]],c);
            }
            ledOnC(&l[HEURE],c);
            break;
    }
    uint8_t mins = minquad(currentTime.min); 
    if(mins > 30){
        ledOnC(&l[MOINS],c);
        mins=60-mins;
    }
    switch(mins){
        case 10 :
            ledOnC(&l[DIX],c);
            break;
        case 20 :
            ledOnC(&l[VINGT],c);
            break;
        case 30 :
            ledOnC(&l[ETDEMIE],c);
            break;
    }
}

void setLedsNb(int8_t nb, Led* l, Tcolor c){
    uint8_t i=0;
    for(;i<NBLEDS;i++){
        ledOff(&l[i]);
    }
    if(nb < 0){
        ledOnC(&l[MOINS],c);
        nb=-nb;
    }
    if(nb>30){
        ledOnC(&l[VINGT],c);
        ledOnC(&l[DIX],c);
        nb=nb-30;
    }else if(nb>20){
        ledOnC(&l[VINGT],c);
        nb=nb-10;
    }else if(nb>11){
        ledOnC(&l[DIX],c);
        nb=nb-10;
    }
    ledOnC(&l[ledMap[nb-1]],c);
}
