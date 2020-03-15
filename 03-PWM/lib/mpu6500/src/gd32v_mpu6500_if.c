#include "gd32v_mpu6500_if.h"
#include "mpu6500_driver.h"
#include "gd32vf103.h"


uint32_t i2c_port;


void mpu6500_install(uint32_t port){
	i2c_port = port;
	i2c_master_init(i2c_port);
	mpu6500_init(&i2c_master_read_register, &i2c_master_write_register, &mpu_delay, (void*)&i2c_port);
}

void i2c_master_init(uint32_t port)
{
    i2c_clock_config(port, 100000, I2C_DTCY_2);
    i2c_enable(port);
}

void mpu_delay(int millis){
    uint64_t start_mtime, delta_mtime;

	// Don't start measuruing until we see an mtime tick
	uint64_t tmp = get_timer_value();
	do {
	start_mtime = get_timer_value();
	} while (start_mtime == tmp);

	do {
	delta_mtime = get_timer_value() - start_mtime;
	}while(delta_mtime <(SystemCoreClock/4000.0 * millis ));
}

void i2c_master_read_register(void* handle, uint8_t addr, uint8_t reg, uint8_t size, uint8_t *data)
{
    int32_t port = *((uint32_t*)handle);
    
    /* send a NACK for the next data byte which will be received into the shift register */
	//i2c_ackpos_config(port, I2C_ACKPOS_NEXT);
    //while(i2c_flag_get(port, I2C_FLAG_I2CBSY));

    /* send a start condition to I2C bus */
    i2c_start_on_bus(port);
    while(!i2c_flag_get(port, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    i2c_master_addressing(port, addr << 1, I2C_TRANSMITTER);
    /* disable ACK before clearing ADDSEND bit */
    //i2c_ack_config(port, I2C_ACK_DISABLE);
    while(!i2c_flag_get(port, I2C_FLAG_ADDSEND));
    i2c_flag_clear(port, I2C_FLAG_ADDSEND);
    while(!i2c_flag_get(port, I2C_FLAG_TBE));


    i2c_data_transmit(port, reg);
    /* wait until the TBE bit is set */
    while(!i2c_flag_get(port, I2C_FLAG_TBE));

    i2c_start_on_bus(port);
    while( ! i2c_flag_get(port, I2C_FLAG_SBSEND) );

    i2c_master_addressing(port, addr << 1, I2C_RECEIVER);
    while( ! i2c_flag_get(port, I2C_FLAG_ADDSEND) );
    i2c_flag_clear(port, I2C_FLAG_ADDSEND);

    i2c_ack_config(port, I2C_ACK_ENABLE);

    for(int i = 0; i < size; i++) {
        if(i == size - 1) {
            i2c_ack_config(port, I2C_ACK_DISABLE);
        }
        while(!i2c_flag_get(port, I2C_FLAG_RBNE));
        *data++ = i2c_data_receive(port);
    }

    i2c_stop_on_bus(port);
    while( I2C_CTL0(port) & I2C_CTL0_STOP );
}


void i2c_master_write_register(void* handle, uint8_t addr, uint8_t reg, uint8_t size, uint8_t *data)
{
    int32_t port = *((uint32_t*)handle);
    
    /* send a NACK for the next data byte which will be received into the shift register */
	//i2c_ackpos_config(port, I2C_ACKPOS_NEXT);
    while(i2c_flag_get(port, I2C_FLAG_I2CBSY));

    /* send a start condition to I2C bus */
    i2c_start_on_bus(port);
    while(!i2c_flag_get(port, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    i2c_master_addressing(port, addr << 1, I2C_TRANSMITTER);
    /* disable ACK before clearing ADDSEND bit */
    //i2c_ack_config(port, I2C_ACK_DISABLE);
    while(!i2c_flag_get(port, I2C_FLAG_ADDSEND));
    i2c_flag_clear(port, I2C_FLAG_ADDSEND);
    while(!i2c_flag_get(port, I2C_FLAG_TBE));

    i2c_data_transmit(port, reg);
        /* wait until the TBE bit is set */
    while(!i2c_flag_get(port, I2C_FLAG_TBE));

    for(int i = 0; i < size; i++){
        i2c_data_transmit(port, *data++);
        /* wait until the TBE bit is set */
        while(!i2c_flag_get(port, I2C_FLAG_TBE));
    }

    i2c_stop_on_bus(port);
    while( I2C_CTL0(port) & I2C_CTL0_STOP );
}