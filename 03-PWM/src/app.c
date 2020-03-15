

#include "gd32vf103.h"
#include "systick.h"
#include <stdlib.h>
#include "math.h"

#define PWMC1_PORT     GPIOA
#define PWMC1_PIN      GPIO_PIN_1
#define PWM_CHANNEL    

#define RED_LED_PIN    GPIO_PIN_13
#define RED_LED_PORT   GPIOC

#define PI  3.14


void init_PWM_example();

int main(void)
{
    /* Hardware PWM on the GD32Vf103 is generted with the help of a timer. It basically compares a number in a register against the current timer count
       and sets the state of the pin low or high based on if the channels value is higher or lower than the current count. */

    /* This example displays a pulsing light in a triangle wave pattern */

    int32_t duty = 0;
    int32_t diff = 4096/128;

    /* Initialize gpio for alternate function */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);

    /* First we need to set up the timer */
    init_PWM_example();

    while(1){

        /* Update pulse width*/
        timer_channel_output_pulse_value_config(TIMER4,TIMER_CH_1,(int)duty);
        
        /* Create triangle wave */
        duty += diff; 

        if(duty > 4096 || duty < 0) diff = -diff; //If the full duty cycle is reached start counting down, if below zero start counting up
        if(duty < 0) duty = 0;                    //Make sure no negative values get written as the dutycycle

        /* Wait a short moment */
        delay_1ms(6);
    }

}

void init_PWM_example(){

    /* These structs are used for configuring the timer */
    timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;

    /* First we need to enable the clock for the timer */
    rcu_periph_clock_enable(RCU_TIMER4);

    /* Reset the timer to a known state */
    timer_deinit(TIMER4);

    /* This function sets the struct up with default values */
    timer_struct_para_init(&timer_initpara);

    /* timer configuration */
    timer_initpara.prescaler         = 1;                   // Prescaler 1 gives counter clock of 108MHz/2 = 54MHz 
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;  // count alignment edge = 0,1,2,3,0,1,2,3... center align = 0,1,2,3,2,1,0
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;    // Counter direction
    timer_initpara.period            = 4095;                // Sets how far to count. 54MHz/4096 = 13,2KHz (max is 65535)
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;    // This is used by deadtime, and digital filtering (not used here though)
    timer_initpara.repetitioncounter = 0;                   // Runs continiously
    timer_init(TIMER4, &timer_initpara);                    // Apply settings to timer


    /* This function initializes the channel setting struct */
    timer_channel_output_struct_para_init(&timer_ocinitpara);
    /* PWM configuration */
    timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;                   // Channel enable
    timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;                 // Disable complementary channel
    timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;             // Active state is high
    timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;    
    timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;            // Idle state is low
    timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER4,TIMER_CH_1,&timer_ocinitpara);   // Apply settings to channel

    timer_channel_output_pulse_value_config(TIMER4,TIMER_CH_1,0);                   // Set pulse width
    timer_channel_output_mode_config(TIMER4,TIMER_CH_1,TIMER_OC_MODE_PWM0);         // Set pwm-mode
    timer_channel_output_shadow_config(TIMER4,TIMER_CH_1,TIMER_OC_SHADOW_DISABLE);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER4);

    /* start the timer */
    timer_enable(TIMER4);
}