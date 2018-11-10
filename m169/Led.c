#include "Led.h" 


void ledInit(Led* leds){
    for(uint8_t i=0;i<NBLEDS;i++){
        leds[i].row=i%2;//leds[0]=0, leds[1]=1
        if(i<10){
            leds[i].col=i;//0-9
        }else{
            leds[i].col=i-9;//1-9
        }
        leds[i].r=0;
        leds[i].g=0;
        leds[i].b=0;
    }
}
uint32_t getColorByLed(Led l){
    uint32_t d=0;
    if(l.row){
        if(l.r)d|=R2;
        if(l.g)d|=G2;
        if(l.b)d|=B2;
    }else{
        if(l.r)d|=R1;
        if(l.g)d|=G1;
        if(l.b)d|=B1;
    }
    return d;
}

uint32_t getDataByColor(uint8_t r,uint8_t g,uint8_t b,uint8_t mode,Led* leds){
    uint32_t data=0;
    for(uint8_t i=0;i<NBLEDS;i++){
        if(leds[i].row==mode){
            if((leds[i].r && r) || (leds[i].g && g) || (leds[i].b && b)){
                data|=(1<<leds[i].col);
            }
        }
    }
    return data;
}

void ledOff(Led* l){
    l->r=0;
    l->g=0;
    l->b=0;
}
void ledOn(Led* l){
    l->r=255;
    l->g=255;
    l->b=255;
}
uint8_t ledIsOff(Led l){
    return !ledIsOn(l);
}
uint8_t ledIsOn(Led l){
    return l.r || l.g || l.b;
}
