#include "usb_serial_if.h"
#include "gd32vf103.h"
#include "drv_usb_hw.h"
#include "systick.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USB_BUFFER_SIZE 1024
#define TIMEOUT_PRINTF 10000
typedef struct{
    uint8_t buffer[USB_BUFFER_SIZE];
    uint32_t max_size;
    uint32_t index;
}USB_serial_buffer_t;

USB_serial_buffer_t usb_buffer = { .buffer = "", .max_size = USB_BUFFER_SIZE, .index = 0};

uint8_t tx_buffer[64] = "";

usb_core_driver USB_OTG_dev = 
{
    .dev = {
        .desc = {
            .dev_desc       = (uint8_t *)&device_descriptor,
            .config_desc    = (uint8_t *)&configuration_descriptor,
            .strings        = usbd_strings,
        }
    }
};

size_t read_usb_serial(uint8_t* data){
    size_t ret = 0;
    usb_buffer.index = 0;
    if (USBD_CONFIGURED == USB_OTG_dev.dev.cur_status){
        while(packet_receive == 1){
            packet_receive = 0;
            usbd_ep_recev (&USB_OTG_dev, CDC_ACM_DATA_OUT_EP, data, 64);
            delay_1ms(1);
            if(receive_length > 0){
                memcpy(&usb_buffer.buffer[usb_buffer.index], data, receive_length);
                usb_buffer.index += receive_length;
                ret += receive_length;
            }
            
        }
        usb_buffer.buffer[usb_buffer.index++] = '\0';
        memcpy(data, usb_buffer.buffer, usb_buffer.index);
    }
    return usb_buffer.index;  
}


//TODO move to USB library
void configure_usb_serial(){

    eclic_global_interrupt_enable();
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL2_PRIO2);
    usb_rcu_config();
    usb_timer_init();
    usb_intr_config();
    usbd_init (&USB_OTG_dev, USB_CORE_ENUM_FS, &usbd_cdc_cb);
}

int usb_serial_available(){return USBD_CONFIGURED == USB_OTG_dev.dev.cur_status;}

#ifdef USE_USB_PRINTF
ssize_t _write(int fd, const void* ptr, size_t len){
    int64_t temp = get_timer_value();
    while(packet_sent == 0 && temp + TIMEOUT_PRINTF > get_timer_value());
    packet_sent = 0;
    strncpy((char*)tx_buffer, ptr, len);
    usbd_ep_send(&USB_OTG_dev, CDC_ACM_DATA_IN_EP, tx_buffer, len);
    return len;
}
#endif