#include "Menu.h"



uint16_t test(int8_t a){
	return a+1;
}

uint16_t MTimeHour(int8_t a) {
	Time_T t=clock.getTime();
	t.hour+=a;
	if(t.hour>23)t.hour=0;
	if(t.hour<0)t.hour=23;
	clock.setTime(t);
	return t.hour;
}
uint16_t MTimeMin(int8_t a) {
	Time_T t=clock.getTime();
	t.min+=a;
	if(t.min>59)t.min=0;
	if(t.min<0)t.min=59;
	clock.setTime(t);
	return t.min;
}

uint16_t MTimeDate(int8_t a) {
	Time_T t=clock.getTime();
	t.date+=a;
	if(t.date==31){
		t.date=1;
	}else if(t.date==0){
		t.date=31;
	}
	clock.setTime(t);
	return t.date;
}
uint16_t MTimeMonth(int8_t a) {
	Time_T t=clock.getTime();
	t.month+=a;
	if(t.month==13){
		t.month=1;
	}else if(t.month==0){
		t.month=12;
	}
	clock.setTime(t);
	return t.month;
}
uint16_t MTimeYear(int8_t a) {
	Time_T t=clock.getTime();
	t.year+=a;
	if(t.year<2000){
		t.year=2000;
	}
	clock.setTime(t);
	return t.year;
}

void initMenus() {
	clock.root=(Menu*)malloc(sizeof(Menu));
	strcpy(clock.root->name,"Menu");
	clock.root->subs=clock.subs;
	clock.root->nbSubs=4;

	for(int i=0;i<4;i++){
		clock.subs[i]=(Menu*)malloc(sizeof(Menu));
		clock.subs[i]->parent=clock.root;
		clock.subs[i]->nbSubs=0;
		clock.subs[i]->subs=clock.l[i];
		clock.subs[i]->nbSubs=5;

	}
	strcpy(clock.subs[0]->name,"Temp");
	strcpy(clock.subs[1]->name,"alar");
	strcpy(clock.subs[2]->name,"conf");
	strcpy(clock.subs[3]->name,"Rst");


	for(int j=0;j<4;j++){
		for(int i=0;i<5;i++){
			clock.l[j][i]=(Menu*)malloc(sizeof(Menu));
			strcpy(clock.l[j][i]->name,"aaaa");
			clock.l[j][i]->name[0]=j+'1';
			clock.l[j][i]->name[3]=i+'1';
			clock.l[j][i]->parent=clock.subs[j];
			clock.l[j][i]->nbSubs=0;
			clock.l[j][i]->fptr=&test;
		}
	}
	strcpy(clock.l[0][0]->name,"Heur");
	clock.l[0][0]->fptr=&MTimeHour;
	strcpy(clock.l[0][1]->name,"Min ");
	clock.l[0][1]->fptr=&MTimeMin;
	strcpy(clock.l[0][2]->name,"Date");
	clock.l[0][2]->fptr=&MTimeDate;
	strcpy(clock.l[0][3]->name,"Mois");
	clock.l[0][3]->fptr=&MTimeMonth;
	strcpy(clock.l[0][4]->name,"Anne");
	clock.l[0][4]->fptr=&MTimeYear;


	clock.cMenu=clock.root;
	clock.currentsub=0;


}
