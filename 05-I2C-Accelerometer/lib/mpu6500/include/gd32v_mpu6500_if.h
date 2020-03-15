#ifndef MPU6500_IF
#define MPU6500_IF

#include "mpu6500_driver.h"




#define I2C_MASTER_SCL_IO 			19               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 			18               /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM 				1					 /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 			400000	        /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 	0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 	0 				/*!< I2C master doesn't need buffer */

#define WRITE_BIT                          I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT                           I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN                       0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS                      0x0              /*!< I2C master will not check ack from slave */
#define ACK_VAL                            0x0              /*!< I2C ack value */
#define NACK_VAL 0x1 /*!< I2C nack value */



void mpu6500_install(uint32_t port);

void i2c_master_init(uint32_t port);

void mpu_delay(int millis);

void i2c_master_read_register(void* handle, uint8_t addr, uint8_t reg, uint8_t size, uint8_t *data);


void i2c_master_write_register(void* handle, uint8_t addr, uint8_t reg, uint8_t size, uint8_t *data);

#endif