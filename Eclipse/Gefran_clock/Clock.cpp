#include "Clock.h"

Clock::	Clock(uint16_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t min, uint8_t sec){

	ASSR|=1<<AS2;
	TCNT2 =0;
	TCCR2B =(1<<CS20)|(1<<CS22);
	TIMSK2|=(1<<TOIE2);

	displayMode=MTIME;
	display.setLed(0,1);
	b[0].pin=UPKEY;
	b[1].pin=DOWNKEY;
	b[2].pin=FKEY;
	b[3].pin=PWRKEY;
	for(uint8_t i=0;i<4;i++){
		b[i].conf=0;
		b[i].state=0;
	}
	KEYREG&=~(b[0].pin | b[1].pin | b[2].pin | b[3].pin);
	KEYPORT|=(b[0].pin | b[1].pin | b[2].pin | b[3].pin);

	PCICR |=(1<<PCIE2);
	PCMSK2 |=(b[3].pin);

	TimeVal.year=year;
	TimeVal.month=month;
	TimeVal.date=date;
	TimeVal.hour=hour;
	TimeVal.min=min;
	TimeVal.sec=sec;

}
Clock::~Clock(){

}


void Clock::setTime(Time_T t){
	TimeVal=t;
}
void Clock::setAlarm(uint8_t h,uint8_t m, bool set){
	AlarmVal.hour=h;
	AlarmVal.min=m;
	AlarmVal.isSet=set;
}

Time_T Clock::getTime(void){
	return TimeVal;
}
Alarm_T Clock::getAlarm(void){
	return AlarmVal;
}

/*Time_T Clock::getTimeIncrement(Time_T t,uint8_t hour, uint8_t min, uint8_t sec){
	t.sec+=sec;
	if(t.sec>=60){
		t.sec=t.sec%60;
		t.min++;
	}
	t.min+=min;
	if(t.min>=60){
		t.min=t.min%60;
		t.hour++;
	}
	t.hour+=hour;
	if(t.hour>=24)t.hour=t.hour%24;
	return t;
}*/
void Clock::incrementTimeSec(void){
	if (++TimeVal.sec==60){
		TimeVal.sec=0;
		if (++TimeVal.min==60){
			TimeVal.min=0;
			if (++TimeVal.hour==24){
				TimeVal.hour=0;
				if (++TimeVal.date==32){
					TimeVal.month++;
					TimeVal.date=1;
				}else if (TimeVal.date==31){
					if ((TimeVal.month==4) || (TimeVal.month==6) || (TimeVal.month==9) || (TimeVal.month==11)){
						TimeVal.month++;
						TimeVal.date=1;
					}
				}else if (TimeVal.date==30){
					if(TimeVal.month==2){
						TimeVal.month++;
						TimeVal.date=1;
					}
				}else if (TimeVal.date==29){
					if((TimeVal.month==2) && (not_leap())){
						TimeVal.month++;
						TimeVal.date=1;
					}
				}
				if (TimeVal.month==13){
					TimeVal.month=1;
					TimeVal.year++;
				}
			}
		}
	}
}
void Clock::addTimeSec(void){
	incrementTimeSec();
	updateDisplay();
	if(TimeVal.sec==0 && TimeVal.min==AlarmVal.min && TimeVal.hour==AlarmVal.hour){
		alarm();
	}

}

void Clock::updateDisplay(void) {
	if(displayMode==MOFF){
		for(uint8_t i=0;i<8;i++)
			display.clearDigit(i);
		display.clearLeds();
		return;
	}
	display.setNum(TimeVal.hour,0,0);
	display.setNum(TimeVal.min,0,1);
	display.setDP(1,1);
	display.setDP(5,1);
	display.setLed(4,AlarmVal.isSet);
	if(displayMode==MTIME){
		display.setNum(TimeVal.date,1,0);
		display.setNum(TimeVal.month,1,1);
	}else if(displayMode==MALARM){
		display.setNum(AlarmVal.hour,1,0);
		display.setNum(AlarmVal.min,1,1);
	}
}


void Clock::alarm() {
	display.setLed(6,1);
	AlarmVal.isActive=true;
	buzz.start();
}

bool Clock::not_leap(void){
	if (!(TimeVal.year%100)){
		return (TimeVal.year%400);
	}else{
		return (TimeVal.year%4);
	}
}
void Clock::checkKeys(void){
	for(uint8_t i=0;i<4;i++){
		if( !(KEYPIN & b[i].pin)){
			b[i].conf++;
		}else{
			b[i].state=0;
			b[i].conf=0;
		}
		if(b[i].state == 0 && b[i].conf>BUTTON_DEB){
			b[i].state=1;
			b[i].conf=0;
			keyPressed(b[i].pin,1);
		}
		if(b[i].state == 1 && b[i].conf>BUTTON_DEB2){
			b[i].state=2;
			keyPressed(b[i].pin,2);
		}
	}
}

void Clock::keyPressed(uint8_t key, uint8_t state){
	if(AlarmVal.isActive){
		display.setLed(6,0);
		buzz.stop();
		AlarmVal.isActive=0;
	}else if(key==FKEY){
		display.setLed(displayMode-1,0);
		displayMode++;
		if(displayMode>2)displayMode=1;
		display.setLed(displayMode-1,1);
	}else if(key==UPKEY && displayMode==MALARM){
		if (++AlarmVal.min==60){
			AlarmVal.min=0;
			if (++AlarmVal.hour==24){
				AlarmVal.hour=0;
			}
		}
	}else if(key==DOWNKEY && displayMode==MALARM){
		if (--AlarmVal.min<0){
			AlarmVal.min=59;
			if (--AlarmVal.hour<0){
				AlarmVal.hour=23;
			}
		}
	}else if(key==PWRKEY && state==1){
		AlarmVal.isSet=!AlarmVal.isSet;
	}else if(key==PWRKEY && state==2){
		if(displayMode==0){
			displayMode=1;
		}else{
			displayMode=0;
		}
	}
	updateDisplay();
}
