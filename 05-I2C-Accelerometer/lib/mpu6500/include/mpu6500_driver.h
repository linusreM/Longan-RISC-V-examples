#ifndef MPU6500_DRIVER
#define MPU6500_DRIVER

#include "mpu6500_registers.h"
#include <stdio.h>
#include <stdint.h>


typedef struct{
	float x;
	float y;
	float z;
}mpu_vector_t;


typedef struct{
	int16_t x;
	int16_t y;
	int16_t z;
}mpu_raw_vector_t;

/** Initialize mpu6500 driver to be portable this is initialized with the functions used for:
 *  read(i2c), write(i2c), and delay
 * 	read should have the structure: read(void* handle, uint8_t addr, uint8_t reg, uint8_t size, uint8_t *data)
 *  write should have the structure: write(void* handle, uint8_t addr, uint8_t reg, uint8_t size, uint8_t *data)
 * 	delay should have the structure: delay(millis)
 * */

int mpu6500_init(void (*read_cb)(void* pHandle, uint8_t addr, uint8_t reg, uint8_t size, uint8_t* pData), 
				 void (*write_cb)(void* pHandle, uint8_t addr, uint8_t reg, uint8_t size, uint8_t* pData), 
				 void (*delay_cb)(int millis),
				 void* pHandle);


/** Gets accelleration data from the sensor
 *  @param pAccel Buffer to store the result in. For standard configuration this is caled to 1 = 1/4096 g 
 * */
int mpu6500_getAccel(mpu_vector_t* pAccel);
/** Gets rotation data from the sensor 
 *  @param pGyro buffer to store result in
 * */
int mpu6500_getGyro(mpu_vector_t* pGyro);

/** 
 * Get Both gyro and accelleration data
 * @param pAccel buffer to store accelleration data in
 * @param pGyro buffer to store rotation data in
*/
int mpu6500_getGyroAccel(mpu_vector_t* pAccel, mpu_vector_t* pGyro);

#endif