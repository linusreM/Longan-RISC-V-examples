#include "gd32vf103.h"
#include "gd32v_tf_card_if.h"
#include "tf_card.h"

fat_time_t time = {.year = 2020, .month = 3, .day = 6, .hour = 0, .minute = 0};

void set_fattime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second){
    time.year = year;
    time.month = month;
    time.day = day;
    time.hour = hour;
    time.minute = minute;
    time.second = second;
}

DWORD get_fattime (void){
	return
		(((uint8_t)(time.year - 1980))  << 25) |
		(time.month	                    << 21) |
		(time.day	                    << 16) |
		(time.hour	                    << 11) |
		(time.minute	                <<  5) |
		(time.second	                <<  0);
}  