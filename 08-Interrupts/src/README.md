<br>

## Interrupt Example

<br>

GD32VF103 has a vast amount of different types of interrupts. This example shows how to configure two types of interrupts.
The first type is a timer triggered interrupt. The timer can be used to generate periodic interrupts which can be used for a great number of different things. The second type is an interrupt which triggers when a specific pin goes from high (typically 3.3V) to low (0V). The obvious use case here is for a button, but can be useful for many different inputs.

The example timer interrupt generates an interrupt each millisecond and increases a variable as a milisecond count. On each even 1000th count the interrupt will also toggle the blue LED on the longan board.

Much of the timer configuration is similar to the PWM example so refer to that example for some extra information on initiating a timer.

<br><br>

### Functions
```c
void TIMER1_IRQHandler(void)

void EXTI5_9_IRQHandler(void)
```
These functions are respectively the handlers for the timer and edge triggered interrupt. The API for GD32VF103 links the names for these functions with the address of the interrupt. This means that if you have the corresponding function defined in your program the interrupt will automatically land in this call.

```c
void eclic_irq_enable(uint32_t source, uint8_t level, uint8_t priority);
```
This function is used to enable an interrupt request. The available interrupts on the chip can be seen in the "IRQn" enum in **gd32vf103.h**. The level refers to the priority level of the interrupt, that is, if an interrupt has a lower level it can preempt a higher level interrupt. Priority refers to the sub-priority and handles which interrupt goes first within a level. For most applications just using priority levels will be sufficient. For more info on priority grouping you can refer to the ARM NVIC which is implemented in the same manner.

By default there are 16 levels and no priority levels but this can be managed using **eclic_priority_group_set(ECLIC_PRIGROUP_LEVELX_PRIOY)**. 

```c
void timer_interrupt_enable(uint32_t timer_periph, uint32_t interrupt)

void timer_interrupt_flag_clear(uint32_t timer_periph, uint32_t interrupt)
```
Timer interrupt enable connects the timer so that an interrupt can be generated. After enabling the interrupt also make sure that the flag is clear so that it doesn't get blocked from triggering.
```c
void gpio_exti_source_select(uint8_t output_port, uint8_t output_pin)
```
The edge triggered interrupts can have multiple sources so this function is used to select which inputs will be connected to the interrupt.
```c
void exti_init(exti_line_enum linex, exti_mode_enum mode, exti_trig_type_enum trig_type)
```
This function selects the behaviour of the interrupt. Mode selects wether the EXTI should interrupt and call a service routine or just set the flag. trig_type selects in what edge it should trigger. It can be falling edge, rising edge or both.

