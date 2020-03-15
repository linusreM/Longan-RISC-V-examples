#include "gd32vf103.h"
#include "systick.h"
#include <stdlib.h>

#define RED_LED_PORT GPIOC
#define RED_LED_PIN GPIO_PIN_13

#define GREEN_LED_PORT GPIOA
#define GREEN_LED_PIN GPIO_PIN_2

void EXTI10_15_IRQHandler(void);
void EXTI5_9_IRQHandler(void);

void eclic_interrupt_config();
void timer_interrupt_config();
void gpio_interrupt_config();

/* Although it is mostly a good idea to avoid using globals
   interrupts often need to communicate by globals since that is the only shared
   interface to the main loop. */
volatile int32_t millis = 0;

int main(void)
{
    /* This example sets up both a timer interrupt which goes off each millisecond, and
       one interrupt which uses a GPIO pin as it's trigger. The timer interrupt counts milliseconds
       and toggles the blue LED each second (1000ms) and the GPIO interrupt toggles the red LED each
       time the pin is connected to 0V(GND) */

    /* GPIO LED initialization */
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(RED_LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, RED_LED_PIN);

    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GREEN_LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GREEN_LED_PIN);

    /* Turn on both LEDs */
    gpio_bit_reset(RED_LED_PORT, RED_LED_PIN);
    gpio_bit_reset(GREEN_LED_PORT, GREEN_LED_PIN);

    /* Enable the Enhancement Core-Local Interrupt Controller */
    eclic_global_interrupt_enable();

    /* Timer interrupt configuration */
    timer_interrupt_config();

    /* GPIO interrupt initialization */
    gpio_interrupt_config();

    /* This program happens all in interrupts so just stop here */
    while(1);
}



void timer_interrupt_config(){
    /* This configuration has a lot in common with the PWM configuration since both relies on a timer. 
       this time though instead of generating a puls each timer period we get an interrupt. This example
       generates an interrupt each millisecond to update a counter and when an even count of 1000 (1s)
       the interrupt will toggle an LED. */

    /* Configuration structs */ 
    timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;

    /* Enable the TIMER1 interrupt request */
    eclic_irq_enable(TIMER1_IRQn,1,0);
    
    /* Enable the peripheral clock. */
    rcu_periph_clock_enable(RCU_TIMER1);

    /* Reset the timer */
    timer_deinit(TIMER1);

    /* initialize timer configuration struct */
    timer_struct_para_init(&timer_initpara);

    /* TIMER1 configuration */
    /* Frequency = core clock / ((1+prescaler)*period) = 108MHz / (2*5400) = 1KHz */
    timer_initpara.prescaler         = 1;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 54000;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(TIMER1, &timer_initpara);

    /* initialize TIMER channel output parameter struct */
    timer_channel_output_struct_para_init(&timer_ocinitpara);

    /* Set the channel configuration */
    timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER1, TIMER_CH_0, &timer_ocinitpara);
    
    /* CH0 configuration in OC timing mode */
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, 54000/2);
    timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_TIMING);
    timer_channel_output_shadow_config(TIMER1, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    /* Enable interrupt on the channel */
    timer_interrupt_enable(TIMER1, TIMER_INT_CH0);
    /* Make sure interrupt flag is clear */
    timer_interrupt_flag_clear(TIMER1, TIMER_INT_CH0);

    /* Start the timer */
    timer_enable(TIMER1);
}

/* This is the interrupt service routine. The linker finds the interrupt vector by name
   so "TIMER1_IRQHandler(void){}" will automatically be called if the corrrespondig interrupt is triggered. "*/
void TIMER1_IRQHandler(void)
{   
    /* Check which channel triggered the interrupt */
    if(SET == timer_interrupt_flag_get(TIMER1, TIMER_INT_CH0)){

        millis++; //Add one each time an interrupt happens
        //gpio_bit_write(GREEN_LED_PORT, GREEN_LED_PIN, !gpio_output_bit_get(GREEN_LED_PORT, GREEN_LED_PIN));

        /* Every second toggle the green LED */
        if(millis % 1000 == 0){
            /* Toggle green LED */
            gpio_bit_write(GREEN_LED_PORT, GREEN_LED_PIN, !gpio_output_bit_get(GREEN_LED_PORT, GREEN_LED_PIN));
        }

        /* Clear the interrupt flag so that new interrupts may happen */
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_CH0);
    }
}

void gpio_interrupt_config(){
    
    /* Enable alternate function clock */
    rcu_periph_clock_enable(RCU_AF);

    /* Enable the particular interrupt vector */
    eclic_irq_enable(EXTI5_9_IRQn, 3, 1);

    /* An interrupt vector can hav multiple different sources, this selects the input that should activate it */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOA, GPIO_PIN_SOURCE_6);

    /* This configures the interrupt behaviour, here we want a falling edge triggered interrupt since the button is active low. */
    exti_init(EXTI_6, EXTI_INTERRUPT, EXTI_TRIG_FALLING);

    /* Clear the interrupt flag so that interrupts can occur. */
    exti_interrupt_flag_clear(EXTI_6);

    /* enable the key gpio clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* Configure interrupt pin as input with internal pull up enabled */
    gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
}

void EXTI5_9_IRQHandler(void){
    /* Check which interrupt triggered */
    if (exti_interrupt_flag_get(GPIO_PIN_6) == SET){

        /* Normaly it's not a good idea to put a delay inside an interrupt. 
           but it's added here to avoid problems with contact bounce and still keep the code relatively simple. */
        delay_1ms(50);

        /* Check that gpio is still low, otherwise we assume it's just contact bounce */
        if(!gpio_input_bit_get(GPIOA, GPIO_PIN_6)){
            /* Toggle red LED */
            gpio_bit_write(RED_LED_PORT, RED_LED_PIN, !gpio_output_bit_get(RED_LED_PORT, RED_LED_PIN));
        } 

        /* Clear interrupt flag */
        exti_interrupt_flag_clear(GPIO_PIN_6);
    }
}