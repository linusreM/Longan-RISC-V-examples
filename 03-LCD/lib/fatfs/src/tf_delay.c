#include "systick.h"
#include "gd32vf103.h"


volatile uint64_t tf_current_time = 0;
volatile uint64_t tf_delay_until = 0;

void tf_start_timer(int32_t millis){
    tf_current_time = get_timer_value();
    tf_delay_until = tf_current_time + ((SystemCoreClock/4000)*millis);
}
uint32_t tf_timer_done(void){
    
    tf_current_time = get_timer_value();
    //if(tf_delay_start > tf_delay_until) return tf_delay_start > current_time && current_time > tf_delay_until ? 0 : 1; //On overflow
    if(tf_current_time > tf_delay_until) return 0;   //No overflow
    return 1;
}