# Longan Nano Examples

The examples in this repository are intended as starting points for some of the different peripherals available on the GD32VF103 chip and the additional hardware on the Longan Nano board. 

To make getting started as easy as possible the examples in general only include the most basic basic settings to make a peripheral work. To go further you will need to either look at the firmware library examples which are much more comprehensive, or reference the manual directly. 

## Using the examples

All of the code that's different between the projects is inside the **src/app.c** file. All of the projects has an individual README.md which explains the program.

### Opening

Use PlatformIO to open a project, just open the PIO home page, then press open project and navigate to one of the example directories.

Since all of the project contains the same libraries except for the app.c file it's no problem to mix and match code from different examples.

### Programming

By default the projects are confgured to be programmed using an RV-link (if you have two Longan Nano you can make one into an RV-link by programming it using this guide).

If you want to use some other method for programming you can change the **platform.ini** file to your prefered programming method.

## Acknowledgements

The examples in this repository are based on a number of different sources. 

* [GD32VF103 Firmware Library Examples](https://github.com/riscv-mcu/GD32VF103_Firmware_Library) Main reference
* [GD32V Virtual Com Port](https://github.com/linusreM/GD32V-Virtual-COM-Port) in turn based on
* [CDC_ACM Example](https://github.com/riscv-mcu/GD32VF103_Demo_Suites/tree/master/GD32VF103C_START_Demo_Suites/Projects/04_USBFS/Device/CDC_ACM) For USB base code 
* [Longan_GD32VF_examples](https://github.com/sipeed/Longan_GD32VF_examples) For LCD and RGB-led 
* [FatFs by ChaN](http://elm-chan.org/fsw/ff/00index_e.html) For the sd-card read/write

