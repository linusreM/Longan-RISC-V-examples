

#include "gd32vf103.h"
#include "systick.h"
#include "lcd.h"

#define I2C_DEVICE_ADDR 0x68
#define I2C_READ_REG    0x75
#define I2C_WRITE_REG   0x75

void i2c_read_register(int32_t port, uint8_t addr, uint8_t reg, uint8_t size, uint8_t *data);
void i2c_write_register(int32_t port, uint8_t addr, uint8_t reg, uint8_t size, uint8_t *data);

int main(void)
{
    /* This example implements the hardware I2C interface to communicate with outside I2C devices */
    uint8_t read_buffer[64] = {0};
    uint8_t write_buffer[64] = {33, 0};
    Lcd_Init();
    LCD_Clear(BLACK);
    LCD_ShowString(0, 20, "I2C example", BLUE);

    delay_1ms(100);

    rcu_periph_clock_enable(RCU_I2C0);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);

    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);
    

    i2c_clock_config(I2C0, 100000, I2C_DTCY_2);
    i2c_enable(I2C0);

    i2c_read_register(I2C0, I2C_DEVICE_ADDR, I2C_READ_REG, 1, read_buffer);

    LCD_ShowStr(0,0,"Read: ", GREEN, TRANSPARENT);
    LCD_ShowNum(8*6, 0, read_buffer[0], 3, YELLOW);

    i2c_write_register(I2C0, I2C_DEVICE_ADDR, I2C_READ_REG, 1, write_buffer);

    while(1);

    //i2c_write_register(I2C0, I2C_DEVICE_ADDR, I2C_WRITE_REG, 1, write_buffer);

}


void i2c_read_register(int32_t port, uint8_t addr, uint8_t reg, uint8_t size, uint8_t *data){

    /* send a start condition to I2C bus */
    i2c_start_on_bus(port);
    while(!i2c_flag_get(port, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus with write flag */
    i2c_master_addressing(port, addr << 1, I2C_TRANSMITTER);
    /* Wait for sending address to finish */
    while(!i2c_flag_get(port, I2C_FLAG_ADDSEND));
    i2c_flag_clear(port, I2C_FLAG_ADDSEND);

    /* Send which register to read */
    i2c_data_transmit(port, reg);
    /* wait until the data has been sent */
    while(!i2c_flag_get(port, I2C_FLAG_TBE));

    /* Send new start condition */
    i2c_start_on_bus(port);
    while( ! i2c_flag_get(port, I2C_FLAG_SBSEND) );

    /* Now send address with read flag */
    i2c_master_addressing(port, addr << 1, I2C_RECEIVER);
    while( ! i2c_flag_get(port, I2C_FLAG_ADDSEND) );
    i2c_flag_clear(port, I2C_FLAG_ADDSEND);

    /* Enable acknowledge for receiving multiple bytes */
    i2c_ack_config(port, I2C_ACK_ENABLE);

    /* Receive bytes, read into buffer. */
    for(int i = 0; i < size; i++) {
        if(i == size - 1) {
            /* If last byte, do not send ack */
            i2c_ack_config(port, I2C_ACK_DISABLE);
        }
        while(!i2c_flag_get(port, I2C_FLAG_RBNE));
        *data++ = i2c_data_receive(port);
    }

    i2c_stop_on_bus(port);
    while( I2C_CTL0(port) & I2C_CTL0_STOP );
}


void i2c_write_register(int32_t port, uint8_t addr, uint8_t reg, uint8_t size, uint8_t *data)
{    
    /* send a NACK for the next data byte which will be received into the shift register */
	//i2c_ackpos_config(port, I2C_ACKPOS_NEXT);
    while(i2c_flag_get(port, I2C_FLAG_I2CBSY));

    /* send a start condition to I2C bus */
    i2c_start_on_bus(port);
    while(!i2c_flag_get(port, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    i2c_master_addressing(port, addr << 1, I2C_TRANSMITTER);
    while(!i2c_flag_get(port, I2C_FLAG_ADDSEND));
    i2c_flag_clear(port, I2C_FLAG_ADDSEND);

    
    /* Send which register to write */
    i2c_data_transmit(port, reg);
    /* wait until the TBE bit is set */
    while(!i2c_flag_get(port, I2C_FLAG_TBE));

    /* Send data */
    for(int i = 0; i < size; i++){
        i2c_data_transmit(port, *data++);
        /* wait until the TBE bit is set */
        while(!i2c_flag_get(port, I2C_FLAG_TBE));
    }

    /* Send stop condition */
    i2c_stop_on_bus(port);
    while( I2C_CTL0(port) & I2C_CTL0_STOP );
}