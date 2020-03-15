
#include "usb_serial_if.h"
#include "gd32vf103.h"
#include "systick.h"
#include <stdlib.h>
#include <string.h>


int read_line(char* buffer);

int main(void)
{

    /* GD32VF103 contains dedicated hardware for USB. To make sending data to and from a computer
       the libraries in these examples includes a virtual com port driver. This lets the usb connection
       on the longan nano appear as a serial device which makes communication with a PC fairly simple.
       
       This example will listen for input from a serial terminal and when it you press enter it will print
       back all it received in the same terminal.
       
       To test connect the longan nano to your computer and open a serial terminal (in platformIO press the icon in the bottom of the window)
       and start typing, then press enter. */

    int count, accumulate = 0;
    char receive_string[128] = {'\0'};
    char print_string[128] = {'\0'};
    
    /* Used for checking if we received a return key */
    char return_key[] = "\r\n";

    /* This sets up so that the longan nano will show up as a virtual com port when connected over usb to a computer */
    configure_usb_serial();

    while(1){
        /* If the controller has sucessfully been connected usb_serial_available() will return 1 */
        if(usb_serial_available()){
            /* Check if something has been received, if so read usb serial will return the amount of bytes received */
            count = read_usb_serial((uint8_t*)receive_string);
            if(count > 0){
                /* Accumulate how many characters have been received */
                accumulate += count;
                
                /* Append the text that was received */
                strcat(print_string, receive_string);

                /* If we received a return key */
                if(strpbrk(print_string, return_key)){
                    /* Print the text that was written back to the host */

                    printf("%.2f Received %d bytes: %s", accumulate, print_string);

                    /* Reset strings and count */
                    receive_string[0] = '\0';
                    print_string[0] = '\0';
                    accumulate = 0;
                }
                
            }
            
        }
    }
}