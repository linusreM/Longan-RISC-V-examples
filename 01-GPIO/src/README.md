<br>

# GPIO Example


This project demonstrates how to initialize and use GPIO on GD32VF103 using the vendor API.
The longan nano green and red LEDs are used as outputs while pins A3 and A4 are used as inputs
with internal pull-ups enabled.

The functions can be used for simple digital control of for example LEDs and to read the state of buttons.

The example code itself checks the pins A3 and A4 and blinks the red and green leds according to if they are connected to ground or not.

All the needed code in this project is available in the **app.c** file.
<br><br>

## Functions Used
```c
void rcu_periph_clock_enable(rcu_periph_enum periph)
```
The RCU (reset and clock unit) controls activation of peripherals. So for each peripheral used a corresponding clock enable needs to be called. For this example GPIOA and GPIOB gets activated. The advantage of this is that any peripheral that is unneded doesn't consume any power.

```c
void gpio_init(uint32_t gpio_periph, uint32_t mode, uint32_t speed, uint32_t pin)
```
This function initializes GPIO pins. 

- uint32_t **gpio_periph**
    - Which port to select, use GPIOA, GPIOB, GPIOC et.c.
- uint32_t **mode**
    - Selects pin mode. GD32V103 supports more than just input and output. The following settings can be used:
        - **GPIO_MODE_OUT_PP** - Output with push-pull
        - **GPIO_MODE_OUT_OD** - Output with open-drain
        - **GPIO_MODE_IN_FLOATING** - Input floating
        - **GPIO_MODE_IPD** - Input with internal pull-down
        - **GPIO_MODE_IPU** - Input with internal pull-up
- uint32_t **speed**
    - Speed mode of the GPIO. Controls the slew-rate of the pins, and therefore produces less noise with the lower settings. Use the following:
        - **GPIO_OSPEED_50MHZ**
        - **GPIO_OSPEED_10MHZ**
        - **GPIO_OSPEED_2MHZ**
- uint32_t **pin**
    - Selects which pin to use. Use:
        - **GPIO_PIN*x***

```c
void gpio_bit_write(uint32_t gpio_periph, uint32_t pin, bit_status bit_value)
```
This function is used to write a value to the GPIO. Keep in mind that GD32V103 uses 3.3V logic so logical high = 3.3V and logical low = 0V
- uint32_t **gpio_periph**
    - Same as above, use GPIOA, GPIOB etc
- uint32_t **pin**
    - Same as above use GPIO_PINx
- bit_status **bit_value**
    - Which state to set the pin to. Use:
        - **SET** - Logical high, 3.3V
        - **1** - Logical high
        - **RESET** - Logical low, 0V(GND)
        - **0** - Logical low

```c
FlagStatus gpio_input_bit_get(uint32_t gpio_periph, uint32_t pin)
```
This function is used to read a pins state. Reads can be done on both inputs and outputs. It returns either RESET(0) or SET(1) based on the current state of the pin.

- uint32_t **gpio_periph**
    - GPIO port select, GPIOA, GPIOB et.c.
- uint32_t **pin**
    - GPIO pin, use GPIO_PINx