#ifndef TF_DELAY_H
#define TF_DELAY_H

#include "gd32vf103.h"

//These functions are used for relieving the timer2 HW dependency and instead uses mtime. There is a *slight* performance loss
//But should be less prone to overlapping dependencies such as for USB-serial

void tf_start_timer(int32_t millis);

int tf_timer_done(void);
#endif //TF_DELAY_H