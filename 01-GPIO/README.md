<br>

## Longan nano base project

<br>

This project is intended as a base to work from when using the **Longan Nano**. 

It provides interfaces for the following parts on the board.

* **LCD driver**
    * Formatted text print supported
    * Display images (RGB565)
    * Draw lines, circles, rectangles
* **RGB LED diver**
    * Simple convinience functions to turn LEDs on and off
* **USB serial driver**
    * Optional printf() redirect
    * Interface for reading
* **`TODO`** **SD-card driver**
    * Read/Write to file using FAT file system 

It also includes drivers for the following external chips.

* **MPU6500 6DOF Accellerometer/gyro**


<br>


### Usage

#### Demo program

The included `app.c` contains some code demonstrating the different functions available using the drivers included in the project. When making a new project you can just remove all of the code that is unneeded for your project.

The code is heavily commented to show what the code does. **TODO** The libraries also have documentation in their respective source files.

#### Build flags

If you want to have support for printf() over USB the build flag **`-DUSE_USB_PRINTF`** should be included, add the line under **`-build_flags = `** in the file **`<platform.ini>`** . If it is not needed you can save some space by not using the flag.

<br>

## Inspiration

Additional information on how to program GD32VF103 devices can be found in the following places.

* [GD32VF103 demo suites](https://github.com/riscv-mcu/GD32VF103_Demo_Suites/tree/master/GD32VF103V_EVAL_Demo_Suites/Projects)
    * Contains a number of examples for the different preipherals available on the MCU
* [GD32VF103 demo suites](https://github.com/riscv-mcu/GD32VF103_Firmware_Library/tree/master/Examples)
    * Contains even more example code for peripherals
* [SUSA.net GD32V blog post](https://www.susa.net/wordpress/2019/10/longan-nano-gd32vf103/)
    * A blog post which examines a lot of interesting things about the chip. Also contains a good walkthrough of using I2C with the API.

<br>

## Documents

Listed below are some of the official documents for the processor

* [Manuals / datasheets for GD32V](http://dl.sipeed.com/LONGAN/Nano/DOC) Contains the datasheet and manual for the processor family.
    * [Datasheet GD32VF103](http://dl.sipeed.com/LONGAN/Nano/DOC/GD32VF103_Datasheet_Rev%201.1.pdf) ~60 page device summary
    * [Manual GD32VF103](http://dl.sipeed.com/LONGAN/Nano/DOC/GD32VF103_User_Manual_CN_V1.2.pdf) ~500 page manual, with complete register definitions
* [Longan nano board hardware info](http://dl.sipeed.com/LONGAN/Nano/HDK)
    * [Longan Nano Schematic](http://dl.sipeed.com/LONGAN/Nano/HDK/Longan%20Nano%202663/Longan%20nano%202663(Schematic).pdf)
    * [LCD datasheet](http://dl.sipeed.com/LONGAN/Nano/HDK/0.96%20inch%2080x160%20IPS%20LCD.pdf)

## Longan nano pinout
    
![Pinout longan nano](http://longan.sipeed.com/assets/longan_nano_pinout_v1.1.0_w5676_h4000_large.png)

<br>

## Acknowledgements

The code and libraries are based on the following projects.

* [GD32V Virtual Com Port](https://github.com/linusreM/GD32V-Virtual-COM-Port) in turn based on
    * [CDC_ACM Example](https://github.com/riscv-mcu/GD32VF103_Demo_Suites/tree/master/GD32VF103C_START_Demo_Suites/Projects/04_USBFS/Device/CDC_ACM) For USB base code 
    * [Longan_GD32VF_examples](https://github.com/sipeed/Longan_GD32VF_examples) For LCD and RGB-led 

