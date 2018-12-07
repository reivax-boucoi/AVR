#ifndef MENU_H
#define MENU_H
#include "defines.h"
#include "Led.h"

void MsetMode(uint8_t i);
void MsetColorMode(uint8_t i);
void MsetNightMode(uint8_t i);
void MsetNightOff(uint8_t i);
void MsetNightOn(uint8_t i);
void MsetHouram(uint8_t i);
void MsetHourpm(uint8_t i);
void MsetMinam(uint8_t i);
void MsetMinpm(uint8_t i);
void MsetEventMode(uint8_t i);
void Mreset(uint8_t i);
void MsetRainbow(uint8_t i);

struct Menu;
typedef const struct Menu{
    const char* name;
    const char* color;
    const int nb_optn;
    void (*fptr)(int);
    const struct{
        const int led;
        const struct Menu* submenu;
    }sub[];
};
struct Menu M0main;
struct Menu M1mode;
struct Menu M1cmode;
struct Menu M1nightmode;
struct Menu M2offhour;
struct Menu M3onhour;
struct Menu M1setTime;
struct Menu M2sethouram;
struct Menu M2sethourpm;
struct Menu M2setminam;
struct Menu M2setminpm;
struct Menu M1eventMode;
struct Menu M1reset;
struct Menu M1rainbow;

#endif
