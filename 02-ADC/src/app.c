

#include "gd32vf103.h"
#include "systick.h"

#define ANALOG_PORT              GPIOA
#define ANALOG_PIN               GPIO_PIN_3
#define ANALOG_CHANNEL           ADC_CHANNEL_3

#define RED_LED_PIN    GPIO_PIN_13
#define RED_LED_PORT    GPIOC


void init_ADC_example();

int main(void)
{
    uint16_t analog_read = 0;

    /* config ADC */
    init_ADC_example();

    /* initiate the gpio for the red led on the longan board */
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(RED_LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, RED_LED_PIN);

    /* This loop generates a software based PWM signal which has a duty cycle based on the read analog value 
       (if you need an actual PWM signal though check the PWM example where it's done in hardware instead) */
    while(1){
        /* Read analog value on pin */
        analog_read = ADC_RDATA(ADC0);

        /* Red LED on */
        gpio_bit_write(RED_LED_PORT, RED_LED_PIN, 0);

        /* If ADC reads a high voltage delay for a longer time */
        delay_1us(analog_read);

        /* Red LED off */
        gpio_bit_write(RED_LED_PORT, RED_LED_PIN, 1);

        /* Delay for longer if ADC reads a low voltage */
        delay_1us(4096 - analog_read);
    }

    
}

void init_ADC_example(){
    /* enable GPIOC clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* Initialize the GPIO that will be used for ADC. A0-A7 and B0-B1 are connected to an ADC-channel each. */
    gpio_init(ANALOG_PORT, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, ANALOG_PIN);
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);

    /* Select the clock frequency that will be used for the ADC core. Refer to README for more info on what to select. */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV8);

    /* Reset ADC0 configuration. GD32VF103 has two internal ADCs (ADC0, ADC1). */
    adc_deinit(ADC0);

    /* Set the ADCs to work independently. Refer to the manual for the different parallel modes available. */
    adc_mode_config(ADC_MODE_FREE);

    /* Set the conversion mode to continuous. Continious mode lets the ADC take measurements continiously without
       an external trigger. */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);

    /* Sets where padding is applied to the measurement. Data alignment right puts padding bits above MSB */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);

    /* Selects how many channels to convert each time. This can be used to "queue" multiple channels. Here just one channel is selected. */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1);

    /* Set the channel as the first "queued" conversion each time the ADC is activated. */
    adc_regular_channel_config(ADC0, 0, ANALOG_CHANNEL, ADC_SAMPLETIME_13POINT5);

    /* Since we are using continious conversion we do not want to use an external trigger. */
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_NONE);
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);

    /* Enable ADC.*/
    adc_enable(ADC0);

    /* Let ADC stabilize */
    delay_1ms(1);

    /* Calibrates the ADC against an internal source. */
    adc_calibration_enable(ADC0);

    /* Start converting */
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}