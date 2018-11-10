#include "RTC.h" 

void setcurrentTime(uint8_t hour,uint8_t min,uint8_t minquad,uint8_t monthDay,uint8_t month){
    currentTime.hour=hour;
    currentTime.min=min;
    currentTime.minquad=minquad;
    currentTime.monthDay=monthDay;
    currentTime.month=month;
}
