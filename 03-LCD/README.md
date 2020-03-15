<br>

# ADC Example


This project demonstrates how to make a basic configuration of the internal ADC. To get the analog signal for the ADC a potentiometer can be connected to the longan nano in the following manner.

Potentiometer:
- Pin1 <---> VCC
- Pin2 <---> A4
- Pin3 <---> GND

The program lets the potentiometer act like a "dimmer" for the red LED on the longa board.
It measures the analog voltage on pin A3 and generates a PWM signal with a duty cycle based on the voltage measured.

All the needed code in this project is available in the **app.c** file. All of the code related to configuring the ADC is located in

```c
void init_ADC_example()
```

This example only covers the most basic just to get the ADC to take a measurement. To get a more full understanding of how the ADC works the manual (chapter 11, pg 153) has a lot of information on the different settings and modes available.
<br><br>

## Important Functions Used
### ADC Initialization
```c
void rcu_adc_clock_config(uint32_t adc_psc)
```
This sets the ADC peripheral clock prescaler. Since the ADC in GD32VF103 has a maximum samplerate at 2MHz (for 12bit) this prescaler is typically needed to bring the sampling speed from the usually much higher core clock. For this project the core clock is 108MHz. With an 8x prescaler that lets the ADC clock run at 13.5MHz.

```c
void adc_mode_config(uint32_t mode)
```
This function sets the relationship between the two ADC's ADC0 and ADC1. Although this example sets them as independent with **ADC_MODE_FREE** you can also set the two ADCs to take measurements in synch, or in an interleaved fashion.


```c
void adc_special_function_config(uint32_t adc_periph, uint32_t function, ControlStatus newvalue)
```
This function has two purposes, it configures scan mode, and wether conversion should be continous. Scan mode refers to wether the ADC should "scan" over multiple channels. Continuity sets if the ADC should keep converting after having completed a conversion.
- uint32_t **adc_periph**
    - Selects which ADC to apply the setting to.
- uint32_t **function**
    - Selects which function to enable or disable. Multiple functions can be selected using bitwise OR. Available options can be found in the manual and the **gd32vf103_adc.h/.c** files.
- ControlStatus **newvalue**
    - **ENABLE** or **DISABLE**

```c
void adc_data_alignment_config(uint32_t adc_periph, uint32_t data_alignment)
```
Sets the alignment of the padding bits in the ADC conversion. Since the ADC doesn't fill an entire 16bits you can select where zeroes are filled in. **ADC_DATAALIGN_RIGHT** fills zeroes above the MSB. **ADC_DATAALIGN_LEFT** fills in zeroes below the LSB.

```c
void adc_channel_length_config(uint32_t adc_periph, uint8_t adc_channel_group, uint32_t length)
```
When in scanning mode this lets you select how many channels to scan over. For the example scanning mode is not used though so just one is selected. Refer to the manual for mor information on using scan modes.

```c
void adc_regular_channel_config(uint32_t adc_periph, uint8_t rank, uint8_t adc_channel, uint32_t sample_time)
```
This function places an ADC channel in the scan queue. Since we are only using one channel in the example we place our channel at the zeroeth rank which would be the first channel measured. This function also selects the sample time, for the example we used a 1MHz sample time. The sample time is calculated as:

Sample_rate = Core_clock/(prescaler * sample_time)<br>
In the example code: 1MHz = 108MHz/(8*13.5) 

uint32_t **sample_time**
- Can be set to the following values
    - **ADC_SAMPLETIME_1POINT5**
    - **ADC_SAMPLETIME_7POINT5**
    - **ADC_SAMPLETIME_13POINT5**
    - **ADC_SAMPLETIME_28POINT5**
    - **ADC_SAMPLETIME_41POINT5**
    - **ADC_SAMPLETIME_55POINT5**
    - **ADC_SAMPLETIME_71POINT5**
    - **ADC_SAMPLETIME_239POINT5**
```c
void adc_external_trigger_source_config(uint32_t adc_periph, uint8_t adc_channel_group, uint32_t external_trigger_source)
```
An external trigger can be selected for ADC conversions. For example a timer can be set as the trigger to start a conversion each time it overflows. The example however does not utilize an external trigger so it is set to NONE. Check the manual for how to set an external trigger as the source.

```c
void adc_calibration_enable(uint32_t adc_periph)
```
This function calibrates the ADC against a known voltage (inside the MCU) use a delay to make sure that the input voltage has stabilized after a reset.


```c
void adc_software_trigger_enable(uint32_t adc_periph, uint8_t adc_channel_group)
```
This function triggers a conversion to start. Since the example uses continious conversion this only needs to be called once. When using software trigger in non-continious mode this needs to be called each time a conversion should occur.

```c
ADC_RDATA(adcx)
```
This is a macro which points at the finished conversion. To read the result of a conversion use for example:

- uint16_t analog_value = ADC_RDATA(ADC0);

