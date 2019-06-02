#ifndef CLOCK_H_
#define CLOCK_H_
#include "Display.h"
#include "Buzzer.h"
#include <avr/io.h>

#define KEYPIN PINC
#define KEYPORT PORTC
#define KEYREG DDRC
#define UPKEY 1<<5
#define DOWNKEY 1<<4
#define FKEY 1<<3
#define PWRKEY 1<<2
#define BUTTON_DEB 20
#define BUTTON_DEB2 500

#define MOFF 0
#define MTIME 1
#define MALARM 2

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
	void setAlarm(uint8_t h,uint8_t m,bool set);
	Time_T getTime(void);
	Alarm_T getAlarm(void);
	Buzzer buzz;
	//Time_T getTimeIncrement(Time_T t,uint8_t hour, uint8_t min, uint8_t sec);
	void addTimeSec(void);
	bool not_leap(void);
	void checkKeys(void);

	~Clock();

private :
	Time_T TimeVal;
	Alarm_T AlarmVal;
	Button_T b[4];
	uint8_t displayMode;
	void updateDisplay();
	void alarm();
	void incrementTimeSec(void);
	void keyPressed(uint8_t key, uint8_t state);

};

#endif
