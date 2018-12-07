#include "Menu.h"
const struct Menu M0main={7,0,{
    {1,&M1mode},
    {2,&M1cmode},
    {3,&M1nightmode},
    {4,&M1setTime},
    {5,&M1eventMode},
    {6,&M1reset},
    {7,&M1rainbow}    
}};
const struct Menu M1mode={5,MsetMode,{
    {1,0},//Hour only
    {2,0},//Hour + temp (slow)
    {3,0},//Hour + temp (fast)
    {4,0},//temp only
    {HEURE,&M0main}//return    
}};
const struct Menu M1cmode={8,MsetColorMode,{
    {1,0},//fixed
    {2,0},//random single (slow)
    {3,0},//random single (fast)
    {4,0},//random all (slow)
    {5,0},//random all (fast)
    {6,0},//cycle (slow)
    {7,0},//cycle (fast)
    {HEURE,&M0main}//return    
}};
const struct Menu M1nightmode={4,MsetNightMode,{
    {1,0},//none
    {2,&M2offhour},//Half brightness
    {3,&M2offhour},//Off completely
    {HEURE,&M0main}//return
}};
const struct Menu M2offhour={12,MsetNightOff,{
    {1,&M3onhour},//heures
    {2,&M3onhour},
    {3,&M3onhour},
    {4,&M3onhour},
    {5,&M3onhour},
    {6,&M3onhour},
    {7,&M3onhour},
    {8,&M3onhour},
    {9,&M3onhour},
    {10,&M3onhour},
    {11,&M3onhour},
    {HEURE,&M1nightmode}//return
}};
const struct Menu M3onhour={12,MsetNightOn,{
    {1,0},//heure
    {2,0},
    {3,0},
    {4,0},
    {5,0},
    {6,0},
    {7,0},
    {8,0},
    {9,0},
    {10,0},
    {11,0},
    {HEURE,&M1nightmode}//return
}};
const struct Menu M1setTime={5,0,{
    {1,&M2sethouram},//hour am
    {2,&M2sethourpm},//hour pm
    {3,&M2setminam},//min am
    {4,&M2setminpm},//min pm
    {HEURE,&M0main}//return
}};
const struct Menu M2sethouram={13,MsetHouram,{
    {1,0},//heure
    {2,0},
    {3,0},
    {4,0},
    {5,0},
    {6,0},
    {7,0},
    {8,0},
    {9,0},
    {10,0},
    {11,0},
    {MIDI,0},//midi
    {HEURE,&M1setTime}//return
}};
const struct Menu M2sethourpm={13,MsetHourpm,{
    {1,0},//heure
    {2,0},
    {3,0},
    {4,0},
    {5,0},
    {6,0},
    {7,0},
    {8,0},
    {9,0},
    {10,0},
    {11,0},
    {MINUIT,0},//minuit
    {HEURE,&M1setTime}//return
}};
const struct Menu M2setminam={4,MsetMinam,{
    {DIX,0},//dix
    {VINGT,0},//vingt
    {ETDEMIE,0},//1/2
    {HEURE,&M1setTime}//return
}};
const struct Menu M2setminpm={4,MsetMinpm,{
    {DIX,0},//dix
    {VINGT,0},//vingt
    {ETDEMIE,0},//1/2
    {HEURE,&M1setTime}//return
}};
const struct Menu M1eventMode={3,MsetEventMode,{
    {1,0},//Event on
    {2,0},//Event off
    {HEURE,&M0main}//return
}};
const struct Menu M1reset={3,Mreset,{
    {1,0},//yes
    {HEURE,&M0main}//return
}};
const struct Menu M1rainbow={3,MsetRainbow,{
    {1,0},//Rainbow on
    {2,0},//Rainbow off
    {HEURE,&M0main}//return
}};

void MsetMode(uint8_t i){
}
void MsetColorMode(uint8_t i){
}
void MsetNightMode(uint8_t i){
}
void MsetNightOff(uint8_t i){
}
void MsetNightOn(uint8_t i){
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
}
void MsetRainbow(uint8_t i){
}
