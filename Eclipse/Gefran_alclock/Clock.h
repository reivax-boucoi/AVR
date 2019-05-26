#ifndef CLOCK_H_
#define CLOCK_H_

#include "Display.h"
#include "Buzzer.h"
#include "Menu.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <stdlib.h>
#include <string.h>

#define KEYPIN PINC
#define KEYPORT PORTC
#define KEYREG DDRC
#define UPKEY 1<<5
#define DOWNKEY 1<<4
#define FKEY 1<<3
#define PWRKEY 1<<2
#define PWRCHECK 1<<0

#define BUTTON_DEB 20
#define BUTTON_DEB2 500

#define MOFF 0
#define MTIME 1
#define MMENU 2

class Menu;//defined in "Menu.h"

struct Time_T{
    uint16_t year;
    uint8_t month;
    uint8_t date;
    int8_t hour;
    int8_t min;
    int8_t sec;
};
struct Alarm_T{
    int8_t hour;
    int8_t min;
    bool isSet;
    bool isActive;
};

struct Button_T{
	uint8_t pin;
	uint8_t state;
	uint16_t conf;
};

class Clock {
public:
	Clock(uint16_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t min, uint8_t sec);
	Display display;
	void setTime(Time_T t);
	void setAlarm(uint8_t h,uint8_t m,bool set);;
	void setAlarm(Alarm_T a);
	void setDisplayMode(uint8_t mode);
	Time_T getTime(void);
	Alarm_T getAlarm(void);
	Buzzer buzz;
	Menu* root;
	Menu* cMenu;
	Menu* subs[4];
	Menu* l[4][5];
	int8_t currentsub;
	void addTimeSec(void);
	bool not_leap(void);
	void checkKeys(void);
	void gotoSleep(void);
	volatile bool sleepFlag;
	~Clock();

private :
	Time_T TimeVal;
	Alarm_T AlarmVal;
	Button_T b[4];
	uint8_t displayMode;
	void updateDisplay();
	void alarm();
	void incrementTimeSec(void);
	void keyPressed(uint8_t key, int8_t state);

};

extern Clock clock;
#endif
