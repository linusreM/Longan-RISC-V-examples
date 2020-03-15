

#include "gd32vf103.h"
#include "systick.h"

#define RED_LED_PIN    GPIO_PIN_13
#define GREEN_LED_PIN  GPIO_PIN_1
#define BLUE_LED_PIN   GPIO_PIN_2

#define RED_LED_PORT    GPIOC
#define GREEN_LED_PORT  GPIOA
#define BLUE_LED_PORT   GPIOA

int main(void)
{
    /* GD32VF103 Utilizes clock gating for peripherals so that they don't consume power while not being used
       therefore for each peripheral you intend to use you need to enable the clock for that peripheral.
       Since this example uses GPIO from PORT A and C both need to be activated. */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);

    /* The following two lines initializes the pins connected to the LEDs on the longan board as outputs
       with push-pull. */
    gpio_init(RED_LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, RED_LED_PIN);           
    /* You can init multiple pins on the same port by using bitwise or */
    gpio_init(GREEN_LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GREEN_LED_PIN | BLUE_LED_PIN); 

    /* This configures the A3 and A4 pins as inputs with internal pull ups enabled */
    gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_3 | GPIO_PIN_4);

    /* gpio_bit_write() sets the state of a pin to either 3.3V or 0V(GND). 
       Use SET or 1 for 3.3V, and RESET or 0 for 0V(GND) */
    gpio_bit_write(GPIOA, GPIO_PIN_1, SET);
    gpio_bit_write(GPIOA, GPIO_PIN_2, SET);
    gpio_bit_write(GPIOC, GPIO_PIN_13, SET);
    
    
    while(1){                                                                               // Loop forever
        
        if(gpio_input_bit_get(GPIOA, GPIO_PIN_3)){                                          // Read pin A3
            gpio_bit_write(GPIOA, GPIO_PIN_1, !gpio_input_bit_get(GPIOA, GPIO_PIN_1));      // If A3 is HIGH(SET), toggle A1
        }
        if(gpio_input_bit_get(GPIOA, GPIO_PIN_4)){                                          // Read pin A4                        
            gpio_bit_write(GPIOC, GPIO_PIN_13, (!gpio_input_bit_get(GPIOC, GPIO_PIN_13)));  // If A4 is HIGH(SET), toggle C13
        }
        delay_1ms(500);                                                                     // Wait for half a second
    }
    

    
}
