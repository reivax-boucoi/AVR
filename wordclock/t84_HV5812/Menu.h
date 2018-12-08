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

typedef struct Menu Menu;
 const struct Menu{
    const uint8_t nb_optn; // color encoded as multiples of 16, nb_optn as modulus 16
    void (*fptr)(uint8_t);
    const struct{
        const uint8_t led;
        const struct Menu* submenu;
    }sub[];
};
uint8_t findNextEntry(struct Menu* m,uint8_t i);
struct Menu* getSubMenu(struct Menu* m,uint8_t index);
void EEPROM_write(uint8_t addr, uint8_t data);
uint8_t EEPROM_read(uint8_t addr);
void Minit(void);

const Menu M0main;
const Menu M1mode;
const Menu M1cmode;
const Menu M1nightmode;
const Menu M2offhour;
const Menu M3onhour;
const Menu M1setTime;
const Menu M2sethouram;
const Menu M2sethourpm;
const Menu M2setminam;
const Menu M2setminpm;
const Menu M1eventMode;
const Menu M1reset;
const Menu M1rainbow;

#endif
