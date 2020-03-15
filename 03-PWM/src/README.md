<br>

## PWM Example

<br>

This project shows how to create a PWM signal on GD32V. A timer is used to generate the period and the counter for the PWM signal. The example will generate a PWM triangle wave on the green LED on the longan board.
The example uses a frequency of 13.5KHz, but this can be configured to suit most needs. Two settings affect the frequency of the PWM signal, the prescaler and the period. the frequency can be calculated from:
    <br><br> **frequency** = **core clock** / **(prescaler * period)**<br><br>
The period also sets the resolution of the duty cycle. For example if you want to create a 50Hz signal with as high resolution as possible
<br><br>**108MHz** / **(prescaler * 65535)** = **50Hz** => **prescaler** = **108MHz / 65535 * 50Hz** = **32.959...**<br><br>
Which would give either full resolution (65536) with a prescaler of 33, and about 0.1% error in frequency (~50.06Hz), or prescaler of 36 with a resolution of 60000 and no error excluding core clock error (Which would be miniscule in this context).

### Functions

The configuration for both the timer and channel uses structs to apply and manage settings. Since the timer is a very versatile peripheral the struct takes a vast amount of different settings. Refer to the manual and the official examples for more examples of the different settings available. For the specific settings to make a basic PWM signal you can refer to the source in this example. 

```c
void timer_struct_para_init(timer_parameter_struct* initpara)
void timer_init(uint32_t timer_periph, timer_parameter_struct* initpara)
```
First create a **timer_parameter_struct** and initialize it with default values with **timer_struct_para_init()** then apply the appropriate settings to the members in the timer struct. When done apply the settings to the timer (**timer_periph**) that you want to use with the **timer_init()** function.

```c
void timer_channel_output_struct_para_init(timer_oc_parameter_struct *ocpara)
void timer_channel_output_config(uint32_t timer_periph, uint16_t channel, timer_oc_parameter_struct* ocpara)
```
The channel is configure in the same manner as the timer. Create the **timer_oc_parameter_struct** and initialize the struct. Then apply the appropriate settings and configure the channel.

```c
void timer_channel_output_mode_config(uint32_t timer_periph, uint16_t channel, uint16_t ocmode)
```
Configures the channel mode. For this example we are using PWM0 mode which is appropriate for generating a basic PWM signal.

```c
void timer_channel_output_shadow_config(uint32_t timer_periph, uint16_t channel, uint16_t ocshadow)
```
The shadow-register is used for capture-compare and should be disabled for the PWM signal.

```c
void timer_auto_reload_shadow_enable(uint32_t timer_periph)
```
Enables the timer to automatically reset each time it overflows.
```c
void timer_enable(uint32_t timer_periph)
```
Starts the timer.

```c
void timer_channel_output_pulse_value_config(uint32_t timer_periph, uint16_t channel, uint32_t pulse)
```
After having done a successful configuration use this function to set the duty cycle. The duty cycle will be:
<br><br> **duty cycle** = **pulse** / **period** <br><br>
With the period being the one set in the timer configuration.