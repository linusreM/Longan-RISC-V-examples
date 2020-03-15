

#include "gd32vf103.h"
#include "systick.h"
#include "stdio.h"
#include "string.h"
#include "gd32v_tf_card_if.h"

/* This contains most of the LCD functions */
#include "lcd.h"



int main(void)
{
    
    /* LCD initiation handles SPI configuration
    *  If you don't need the LCD you can check how the LCD initiates SPI in the library */
    Lcd_Init();
    LCD_Clear(BLACK);

    
    /* Handle for the mounted filesystem */
    FATFS fs;

    /* FatFs return code */
    volatile FRESULT fr; 

    /* File handle */    
    FIL fil;       

    /* Used for bytes written, and bytes read */         
    UINT bw = 999;
    UINT br = 0;

    /* A string to write to file */
    char buf[128] = "write this to a file\n";

    /* A buffer for storing an image from the SD-card */
    uint16_t image_buffer[20*30] = {0};

    /* For sequencing numbers in loop */
    uint32_t file_no = 0;

    /* Sets a valid date for when writing to file */
    set_fattime(1980, 1, 1, 0, 0, 0);

    /* This function "mounts" the SD-card which makes the filesystem available */
    fr = f_mount(&fs, "", 1);       //Mount storage device

    /* This function opens a file. In this case we are creating a file which we want to write to */
    fr = f_open(&fil, "file.txt", FA_WRITE);

    /* Write some text to the file */
    for(int i = 0; i < 10; i++) fr = f_write(&fil, buf, strlen(buf), &bw);

    delay_1ms(100);

    /* Close the file */
    f_close(&fil);                                                              //Close file
    
    delay_1ms(500);


    /* Now we open a file for reading. */
    fr = f_open(&fil, "1.bin", FA_READ);

    /* Read 20x30x2 bytes since the images are 16bit 20x30px. The data is written into "image_buffer" */ 
    fr = f_read(&fil, image_buffer, 20*30*2+4, &br);
    f_close(&fil);

    /* "image_buffer" now contains the image and we just have to display it on the screen */
    LCD_ShowPicture(10, 10, 29, 39, (u8*)image_buffer+4);

    /* The rest follows the same procedure */
    fr = f_open(&fil, "2.bin", FA_READ);
    fr = f_read(&fil, image_buffer, 20*30*2+4, &br);
    f_close(&fil);

    LCD_ShowPicture(30, 10, 49, 39, (u8*)image_buffer+4);

    fr = f_open(&fil, "colon.bin", FA_READ);
    fr = f_read(&fil, image_buffer, 20*30*2+4, &br);
    f_close(&fil);

    LCD_ShowPicture(50, 10, 69, 39, (u8*)image_buffer+4);

    fr = f_open(&fil, "3.bin", FA_READ);
    fr = f_read(&fil, image_buffer, 20*30*2+4, &br);
    f_close(&fil);

    LCD_ShowPicture(70, 10, 89, 39, (u8*)image_buffer+4);

    /* We can also select the file dynamically */
    strcpy(buf, "0.bin");

    while(1){

        /* buf[0] will have the value '0' - '9' so the string will loop through "0.bin" -> "1.bin" -> ... -> "9.bin" and so on */
        buf[0] = file_no + '0';
        file_no += 1;
        file_no %= 10;

        fr = f_open(&fil, buf, FA_READ);
        fr = f_read(&fil, image_buffer, 20*30*2+4, &br);
        f_close(&fil);

        LCD_ShowPicture(90, 10, 109, 39, (u8*)image_buffer+4);

        delay_1ms(1000);
    }


}