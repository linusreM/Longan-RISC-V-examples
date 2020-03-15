/*!
    \file  main.c
    \brief USB CDC ACM device

    \version 2019-6-5, V1.0.0, demo for GD32VF103
*/

/*
    Copyright (c) 2019, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "usb_serial_if.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcd.h"
#include "lcd_light_gui.h"
#include "gd32v_pjt_include.h"
#include "kth_logo.h"
#include "gd32v_mpu6500_if.h"
#include "gd32v_tf_card_if.h"

#define GRAPH_HEIGHT    30

int test_tf();


int main(void)
{
    uint8_t tmpbuf[64] = "";            //Used to store usb reads
    mpu_vector_t vec;
    u16 line_color;

    int32_t values_to_graph[80] = { 0, 31, 61, 90, 117, 141, 161, 178, 190, 197, 
                                    200, 197, 190, 178, 161, 141, 117, 90, 61, 31,
                                    0, -31, -61, -90, -117, -141, -161, -178, -190, -197,
                                    -200, -197, -190, -178, -161, -141, -117, -90, -61, -31,
                                    0, 31, 61, 90, 117, 141, 161, 178, 190, 197, 
                                    200, 197, 190, 178, 161, 141, 117, 90, 61, 31,
                                    0, -31, -61, -90, -117, -141, -161, -178, -190, -197,
                                    -200, -197, -190, -178, -161, -141, -117, -90, -61, -31};

    graph_config_t graph_1 = {
        .color = YELLOW,
        .background = BLACK,
        .x_origin = 30, 
        .y_origin = 20,
        .scale_setting = SCALE_TO_FIT,
        .scaling = 1,
        .lower_bound = 0
    };
    graph_config_t graph_2 = {
        .color = BLUE,
        .background = BLACK,
        .x_origin = 30, 
        .y_origin = 20,
        .scale_setting = USE_CUSTOM,
        .scaling = 50,
        .lower_bound = -(50*GRAPH_HEIGHT)
    };

    gui_init_graph(&graph_1, 100, GRAPH_HEIGHT);
    gui_init_graph(&graph_2, 100, GRAPH_HEIGHT);
    
    configure_usb_serial();



    /* Initialize GPIO for LEDs*/
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);           //Pin 13, port C
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1|GPIO_PIN_2); //Pin 1 and 2, Port A

    /* Turning on or off LEDs */
    LEDR(1);
    LEDG(1);
    LEDB(1);

    /* This (global)variable holds the background color for the LCD : TODO: Make a "LCD_set_background(u16 color)" function in the LCD library
       You can use the following link to generate a particular color:
       https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html */
    BACK_COLOR = 0x0000;

    /* Initialize Display */
    Lcd_Init();
    /* Clear lcd to background color */
    
    LCD_Clear(BACK_COLOR);
    LCD_DrawRectangle(30-1, 20-1, 30 + 100+1, 20+30+1, GREEN);
    for(int i = 0; i < 4; i++){
        for(int i = 0; i < 120; i++){
            LCD_ShowNum(0,0,graph_1.scaling, 6,WHITE);
            gui_erase_graph(&graph_1);
            gui_erase_graph(&graph_2);
            gui_render_graph(&graph_1);
            gui_render_graph(&graph_2);
            gui_append_graph(&graph_1, &values_to_graph[i % 40], 1);
            gui_append_graph(&graph_2, &values_to_graph[(i+20) % 40], 1);
            delay_1ms(32);
        }

        for(int i = 0; i < 40; i++) values_to_graph[i] *= 4;
    
        for(int i = 0; i < 120; i++){
            LCD_ShowNum(0,0,graph_1.scaling, 6,WHITE);
            gui_erase_graph(&graph_1);
            gui_erase_graph(&graph_2);
            gui_render_graph(&graph_1);
            gui_render_graph(&graph_2);
            gui_append_graph(&graph_1, &values_to_graph[i % 40], 1);
            gui_append_graph(&graph_2, &values_to_graph[(i+20) % 40], 1);
            delay_1ms(32);
        }
        for(int i = 0; i < 40; i++) values_to_graph[i] /= 2;
    }
    
    

    

    

    delay_1ms(1000);

    test_tf();
    

    /* Print some letters to LCD, First two arguments are the starting X and Y coordinates */
    LCD_ShowStr(0, 0, (u8 *)("LCD+USB-serial demo"), GREEN, OPAQUE);
    LCD_ShowStr(0, 16, (u8 *)("Awaiting USB connection"), BLUE, OPAQUE);
   
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);
    rcu_periph_clock_enable(RCU_I2C0);
    //printf("attempting mpu65000 install\r\n");
    //fflush(0);
    //mpu6500_install(I2C0);
    //printf("mpu65000 installed\r\n");
    //fflush(0);
    //mpu6500_getAccel(&vec);
    //printf("mpu65000 get accel complete\r\n");
    //fflush(0);
    sprintf((char*)tmpbuf, "Z=%d    ", (int)vec.z);
    LCD_ShowStr(0,  48, (u8 *)tmpbuf, WHITE, OPAQUE);    //Display last letter on screen

    /*Sets up USB-serial, if you don't need usb you can remove the next two function calls*/    
    
    /* Wait for USB to be registered by a host */
    while (!usb_serial_available()) {
    }
    //delay_1ms(1000);
    printf("Test sending one rows\r\n\r");fflush(0);
    printf("Test sending two rows\r\n\r");fflush(0);
    printf("Test sending three rows\r\n\r");fflush(0);
    printf("Test sending four rows\r\n\r");fflush(0);
    //read_usb_serial(tmpbuf);
    printf("Connection open:");fflush(0);
    //read_usb_serial(tmpbuf);
    LCD_ShowStr(0,  16, (u8 *)("USB-serial connected"), YELLOW, OPAQUE);
    LCD_ShowStr(0,  32, (u8 *)("Open a terminal"), YELLOW, OPAQUE);

    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);
    rcu_periph_clock_enable(RCU_I2C0);
    
    mpu6500_install(I2C0);
    
    /* Show a picture on the display. */
    LCD_Clear(BACK_COLOR);
    LCD_ShowPicture(110, 0, 159, 49, (uint8_t*)&kth_logo_map);
    
    while(1){
        LEDB(1);            //Blue LED off

        delay_1ms(30);       //Use this function for delays (Though, keep in mind usually its best not to use delays)

        LEDB(0);            //Blue led on

        delay_1ms(30);
        if(read_usb_serial(tmpbuf) != 0){

            printf((char*)tmpbuf); //Print if anything has been received
            fflush(0);                        //Must be called for anything to show up if not using \r\n at the end of the string
        }
        mpu6500_getAccel(&vec);
        //sprintf((char*)tmpbuf, "Z=%d    ", (int)(vec.z));
        //LCD_ShowStr(0,  48, (u8 *)tmpbuf, WHITE, OPAQUE);    //Display last letter on screen
        LCD_Fill((160/2)-28, (80/2)-28,(160/2)+28, (80/2)+28,BLACK);
        line_color = (vec.z < 0) ? RED : GREEN; 
        Draw_Circle(160/2, 80/2, 28, BLUE);
        LCD_DrawLine(160/2, 80/2, (160/2)-(vec.x)/(4096/28), (80/2)+(vec.y/(4096/28)),line_color); //4096 = 1g
    }
}

//Move to USB library


int test_tf(){
    FATFS fs;
    volatile FRESULT fr;     /* FatFs return code */
    FIL fil;
    UINT bw = 999;
    char buf[128] = "lulzzz!!!\n";

    set_fattime(1980, 1, 1, 0, 0, 0);

    fr = f_mount(&fs, "", 1);       //Mount storage device
    if(fr == 0) {                   //If successful mount
        fr = f_open(&fil, "this99.txt", FA_CREATE_ALWAYS | FA_WRITE);               //Create file
        for(int i = 0; i < 1000000; i++) fr = f_write(&fil, buf, strlen(buf), &bw); //Write to file
        f_close(&fil);                                                              //Close file
    }
    return fr;
}
