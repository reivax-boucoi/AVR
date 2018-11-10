#include "Led.h" 

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
