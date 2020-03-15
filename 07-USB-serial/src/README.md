<br>

## USB Serial Example

<br>

The GD32VF103 chip includes hardware support for USB and the longan nano usb port is connected with that interface. USB can be used for a vast amount of purposes such as HID-devices, mass storage devices et.c. To make it easy to communicate between computer and MCU these examples includes an implementation of a virtual com-port to enable serial port communication over usb to a terminal on PC.

To simplify writing values using usb the interface has been implemented to use **printf()**. Since printf uses a fair amount of program space the flags for **USE_USB** and **USE_USB_PRINTF** need to be defined for the compiler to include it in the program. The simplest way to put the flags in is to either use **#define** in the **app.c** file or under **-build_flags** put **-DUSE_USB** and **-DUSE_USB_SERIAL**.

To use the example connect the longan nano via its usb port and open a terminal (platformIO has an included serial terminal, click the icon in the bottom bar with the little plug). Then start typing some in the terminal. When done typing press enter and the program will transmit the message written back to the terminal as well as how many bytes the message contained.

### Functions
<br>

```c
void configure_usb_serial()
```
This function is used to initialize the USB interface to act as a virtual serial port. The driver uses timer2 so make sure when using USB that you are not reconfiguring timer2 to do something else.

```c
size_t read_usb_serial(uint8_t *data)
```
This function will read any data received from the USB interface. If anything has been received it will be collected into the data buffer and ended with a '\0' string terminator. The function will also return the number of bytes received.
```c
void printf()
```
Works like standard printf, but is rerouted through the USB-serial port. Printing floats is currently not supported though so printing floats has to be done manualy (eg. "%d,%d", (int)float_var, (int)((float_var-((int)float_var)))*100.0)

Though keep in mind that the GD32VF103 chip doesn't have dedicated hardware for float calculations so whenever possible using integer calculation instead will be a lot more efficient.