#include "gd32vf103.h"
#include "tf_card.h"


typedef struct{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
}fat_time_t;
/**
 * Set time for FAT filesystem
 * */
void set_fattime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
/** Get time for fat file system
 * */
DWORD get_fattime(void);