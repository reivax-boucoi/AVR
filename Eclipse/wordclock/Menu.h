#ifndef MENU_H
#define MENU_H
#include "defines.h"
#include "Led.h"

void MsetMode(uint8_t i);
uint8_t getMode(void);
void MsetColorMode(uint8_t i);
uint8_t getColor(void);
void MsetNightMode(uint8_t i);
uint8_t isInAllowedTime(uint8_t h);
void MsetNightOff(uint8_t i);
void MsetNightOn(uint8_t i);
void MsetHouram(uint8_t i);
void MsetHourpm(uint8_t i);
void MsetMinam(uint8_t i);
void MsetMinpm(uint8_t i);
void MsetEventMode(uint8_t i);
void Mreset(uint8_t i);
void MsetRainbow(uint8_t i);

//const struct Menu;
typedef const struct Menu{
    const uint8_t nb_optn;
    void (*fptr)(uint8_t);
    const struct{
        const uint8_t led;
        const struct Menu* submenu;
    }sub[];
};
uint8_t findNextEntry(struct Menu* m,uint8_t i);
struct Menu* getSubMenu(struct Menu* m,uint8_t index);

const struct Menu M0main;
const struct Menu M1mode;
const struct Menu M1cmode;
const struct Menu M1nightmode;
const struct Menu M2offhour;
const struct Menu M3onhour;
const struct Menu M1setTime;
const struct Menu M2sethouram;
const struct Menu M2sethourpm;
const struct Menu M2setminam;
const struct Menu M2setminpm;
const struct Menu M1eventMode;
const struct Menu M1reset;
const struct Menu M1rainbow;

#endif
