#include "RTC.h" 

void setCurrentTime(uint8_t hour,uint8_t min,uint8_t monthDay,uint8_t month){
    currentTime.hour=hour;
    currentTime.min=min;
    currentTime.monthDay=monthDay;
    currentTime.month=month;
}

uint8_t minquad(uint8_t min){
	if(min > 5){
		if(min > 15){
			if(min > 25){
				if(min > 35){
					if(min > 45){
						if(min > 55){
							return 0;
						}else{return 50;}
					}else{return 40;}
				}else{return 30;}
			}else{return 20;}
		}else{return 10;}
	}else{return 0;}
}

uint8_t bcdToDec(uint8_t val){
  return ((val/16*10) + (val%16));
}
