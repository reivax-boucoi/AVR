#include "Menu.h"

uint8_t mode=1;//0: hour only, 1-2: hour+temp, 3: temp only
uint8_t colorMode=0;
uint8_t findNextEntry(Menu* m,uint8_t index){
    if((index+2)>(m->nb_optn%16))return 0;
    return index+1;
}

Menu* getSubMenu(Menu* m,uint8_t index){
    if(m->sub[index].submenu==0)return 0;//if executed NULL, return blank menu (==exit setup)
    return m->sub[index].submenu;
}
void EEPROM_write(uint8_t addr, uint8_t data){
    while(EECR & (1<<EEPE));
    EECR = (0<<EEPM1)|(0<<EEPM0);
    EEAR = addr;
    EEDR = data;
    EECR |= (1<<EEMPE);
    EECR |= (1<<EEPE);
}
uint8_t EEPROM_read(uint8_t addr){
    while(EECR & (1<<EEPE));
    EEAR = addr;
    EECR |= (1<<EERE);
    return EEDR;
}

void Minit(void){
    colorMode=EEPROM_read(EE_COLOR);
    MsetMode(EEPROM_read(EE_MODE));
}
const Menu M0main={8,0,{//WHITE
    {7,&M1mode},
    {8,&M1cmode},
    {10,&M1nightmode},
    {14,&M1setTime},
    {0,&M1eventMode},
    {4,&M1reset},
    {17,&M1rainbow},
    {HEURE,0}//exit      
}};
const Menu M1mode={5+16,MsetMode,{//CYAN
    {7,0},//Hour only
    {8,0},//Hour + temp (slow)
    {10,0},//Hour + temp (fast)
    {14,0},//temp only
    {HEURE,&M0main}//return    
}};
const Menu M1cmode={10+32,MsetColorMode,{//BLUE
    {7,0},//fixed WHITE 1
    {8,0},//fixed CYAN 2
    {10,0},//fixed BLUE 3
    {14,0},//fixed MAGENTA 4
    {0,0},//fixed RED 5
    {4,0},//fixed YELLOW 6
    {17,0},//fixed GREEN 7
    {2,0},// cycle single 8
    {12,0},// cycle all 9
    {HEURE,&M0main}//return    
}};
const Menu M1nightmode={4+48,MsetNightMode,{//MAGENTA
    {7,0},//none
    {8,&M2offhour},//Half brightness
    {10,&M2offhour},//Off completely
    {HEURE,&M0main}//return
}};
const Menu M2offhour={12+64,MsetNightOff,{//RED
    {7,&M3onhour},//heures
    {8,&M3onhour},
    {10,&M3onhour},
    {14,&M3onhour},
    {0,&M3onhour},
    {4,&M3onhour},
    {17,&M3onhour},
    {2,&M3onhour},
    {12,&M3onhour},
    {6,&M3onhour},
    {16,&M3onhour},
    {HEURE,&M1nightmode}//return
}};
const Menu M3onhour={12+80,MsetNightOn,{//YELLOW
    {7,0},//heures
    {8,0},
    {10,0},
    {14,0},
    {0,0},
    {4,0},
    {17,0},
    {2,0},
    {12,0},
    {6,0},
    {16,0},
    {HEURE,&M1nightmode}//return
}};
const Menu M1setTime={5,0,{
    {7,&M2sethouram},//hour am
    {8,&M2sethourpm},//hour pm
    {10,&M2setminam},//min am
    {14,&M2setminpm},//min pm
    {HEURE,&M0main}//return
}};
const Menu M2sethouram={13,MsetHouram,{
    {7,0},//heure
    {8,0},
    {10,0},
    {14,0},
    {0,0},
    {4,0},
    {17,0},
    {2,0},
    {12,0},
    {6,0},
    {16,0},
    {MIDI,0},//midi
    {HEURE,&M1setTime}//return
}};
const Menu M2sethourpm={13,MsetHourpm,{
    {7,0},//heure
    {8,0},
    {10,0},
    {14,0},
    {0,0},
    {4,0},
    {17,0},
    {2,0},
    {12,0},
    {6,0},
    {16,0},
    {MINUIT,0},//minuit
    {HEURE,&M1setTime}//return
}};
const Menu M2setminam={4,MsetMinam,{
    {DIX,0},//dix
    {VINGT,0},//vingt
    {ETDEMIE,0},//1/2
    {HEURE,&M1setTime}//return
}};
const Menu M2setminpm={4,MsetMinpm,{
    {DIX,0},//dix
    {VINGT,0},//vingt
    {ETDEMIE,0},//1/2
    {HEURE,&M1setTime}//return
}};
const Menu M1eventMode={3,MsetEventMode,{
    {7,0},//Event on
    {8,0},//Event off
    {HEURE,&M0main}//return
}};
const Menu M1reset={2,Mreset,{
    {18,0},//yes
    {HEURE,&M0main}//return
}};
const Menu M1rainbow={3,MsetRainbow,{
    {7,0},//Rainbow on
    {8,0},//Rainbow off
    {HEURE,&M0main}//return
}};

void MsetMode(uint8_t i){
    if(i<4){
        TCCR1B|=(1<<CS12)|(1<<CS10);
        if(i==2)TCCR1B&=~(1<<CS10);
        mode=i;
        EEPROM_write(EE_MODE,i);
    }
}
uint8_t getMode(void){
    if(mode==2){
        mode=1;
    }else if(mode==1){
        mode=2;
    }
    return mode;
}
void MsetColorMode(uint8_t i){
    if(i<7){
        colorMode=colorArray[i];//fixed
        EEPROM_write(EE_COLOR,colorArray[i]);
    }else if(i==7){//cycle single
        colorMode=WHITE+128;
        EEPROM_write(EE_COLOR,128);
    }else if(i==8){//cycle all
        colorMode=WHITE+64;
        EEPROM_write(EE_COLOR,64);
    }
}
uint8_t getColor(void){
    if(colorMode>127){//cycle single
        if(colorMode>134)colorMode=128;
        colorMode++;
        return colorArray[colorMode-128];
    }else if(colorMode>63){//cycle all
        return RED;
    }else{//fixed
        return colorMode;
    }
}
void MsetNightMode(uint8_t i){
    if(i<3)EEPROM_write(EE_NIGHTMODE,i);
}
uint8_t isInAllowedTime(uint8_t h){
    if(EEPROM_read(EE_NIGHTMODE))return (h>=EEPROM_read(EE_ONHOUR) && h<=EEPROM_read(EE_OFFHOUR));
    return 1;
}
void MsetNightOff(uint8_t i){
    EEPROM_write(EE_OFFHOUR,12+i);
}
void MsetNightOn(uint8_t i){
    EEPROM_write(EE_ONHOUR,1+i);
}
void MsetHouram(uint8_t i){
}
void MsetHourpm(uint8_t i){
}
void MsetMinam(uint8_t i){
}
void MsetMinpm(uint8_t i){
}
void MsetEventMode(uint8_t i){
}
void Mreset(uint8_t i){
    if(i<1){
        MsetMode(1);
        MsetColorMode(0);
        TCCR1B|=(1<<CS12)|(1<<CS10);
    }
}
void MsetRainbow(uint8_t i){
}
