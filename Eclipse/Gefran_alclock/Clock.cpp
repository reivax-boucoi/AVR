#include "Clock.h"

Clock clock(2019,06,03,0,34,50);

Clock::Clock(uint16_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t min, uint8_t sec){

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
	KEYREG&=~(b[0].pin | b[1].pin | b[2].pin | b[3].pin | PWRCHECK);
	KEYPORT|=(b[0].pin | b[1].pin | b[2].pin | b[3].pin);

	PCICR |=(1<<PCIE2);
	PCMSK2 |=(1<<PCINT16);//PWRCHECK interrupt

	ADCSRA = 0;

	TimeVal.year=year;
	TimeVal.month=month;
	TimeVal.date=date;
	TimeVal.hour=hour;
	TimeVal.min=min;
	TimeVal.sec=sec;

	root=(Menu*)malloc(sizeof(Menu));
	strcpy(root->name,"Menu");
	root->subs=subs;
	root->nbSubs=4;

	for(int i=0;i<4;i++){
		subs[i]=(Menu*)malloc(sizeof(Menu));
		subs[i]->parent=root;
		subs[i]->nbSubs=0;
		subs[i]->subs=l[i];
		subs[i]->nbSubs=5;

	}
	strcpy(subs[0]->name,"Temp");
	strcpy(subs[1]->name,"alar");
	strcpy(subs[2]->name,"conf");
	strcpy(subs[3]->name,"Rst");


	for(int j=0;j<4;j++){
		for(int i=0;i<5;i++){
			l[j][i]=(Menu*)malloc(sizeof(Menu));
			strcpy(l[j][i]->name,"aaaa");
			l[j][i]->name[0]=j+'1';
			l[j][i]->name[3]=i+'1';
			l[j][i]->parent=subs[j];
			l[j][i]->nbSubs=0;
			l[j][i]->fptr=&test;
		}
	}
	strcpy(l[0][0]->name,"Heur");
	l[0][0]->fptr=&MTimeHour;
	strcpy(l[0][1]->name,"Min ");
	l[0][1]->fptr=&MTimeMin;
	strcpy(l[0][2]->name,"Date");
	l[0][2]->fptr=&MTimeDate;
	strcpy(l[0][3]->name,"Mois");
	l[0][3]->fptr=&MTimeMonth;
	strcpy(l[0][4]->name,"Anne");
	l[0][4]->fptr=&MTimeYear;


	cMenu=root;
	currentsub=0;

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

void Clock::setDisplayMode(uint8_t mode){
	displayMode=mode;
}

void Clock::gotoSleep(void){
	TCCR0B &=~(1<<CS02);
	displayMode=MOFF;
	SEGPORT=0x00;
	COMPORT=0x00;
	COMPORT1=0x00;

	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	sleep_mode();

	TCCR0B |=(1<<CS02);
	displayMode=MTIME;
	updateDisplay();
}

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
	if(TimeVal.sec==0 && TimeVal.min==AlarmVal.min && TimeVal.hour==AlarmVal.hour && AlarmVal.isSet){
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
	if(displayMode==MTIME){
		display.setNum(TimeVal.hour,0,0);
		display.setNum(TimeVal.min,0,1);
		display.setDP(1,1);
		display.setDP(5,1);
		display.setLed(4,AlarmVal.isSet);
		display.setNum(TimeVal.date,1,0);
		display.setNum(TimeVal.sec,1,1);
	}else{
		display.setDP(1,0);
		display.setDP(5,0);
		display.setText(cMenu->name,4,0);
		if(cMenu->nbSubs>0){
			display.setText(cMenu->subs[currentsub]->name,4,1);
		}else{
			display.clearDigit(4);
			display.clearDigit(5);
			display.setNum(cMenu->fptr(0),1,1);
		}
	}

}


void Clock::alarm() {
	sleepFlag=0;
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
	if(AlarmVal.isActive){//alarm clear
		display.setLed(6,0);
		buzz.stop();
		AlarmVal.isActive=0;
		//if(!(KEYPIN & PWRCHECK))sleepFlag=1;//put back to sleep after alarm

	}else if(key==FKEY && displayMode==MTIME){//enter menu mode
		displayMode=MMENU;
	}else if(key==PWRKEY && displayMode==MMENU){//back in menu
		if(cMenu==root){
			currentsub=0;
			displayMode=MTIME;
		}else{
			cMenu=cMenu->parent;
		}
	}else if(key==FKEY && displayMode==MMENU){//enter/execute menu
		if(cMenu->fptr !=nullptr ){
			currentsub=0;
			displayMode=MTIME;
			cMenu=root;
		}else if(cMenu->nbSubs>0){
			cMenu=cMenu->subs[currentsub];
			currentsub=0;
		}
	}else if(key==DOWNKEY && displayMode==MMENU){//next menu
		if(cMenu->nbSubs<=0){
			cMenu->fptr(-1);
		}else{
			currentsub++;
			if(currentsub>=cMenu->nbSubs)currentsub=0;
		}
	}else if(key==UPKEY && displayMode==MMENU){//previous menu
		if(cMenu->nbSubs<=0){
			cMenu->fptr(1);
		}else{
			currentsub--;
			if(currentsub<0)currentsub=cMenu->nbSubs-1;
		}
	}
	updateDisplay();
}
