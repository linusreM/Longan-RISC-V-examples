#include "mpu6500_driver.h"
#include "stdint.h"

typedef struct{
	void (*read)(void* pHandle, uint8_t addr, uint8_t reg, uint8_t size, uint8_t* pData);
	void (*write)(void* pHandle, uint8_t addr, uint8_t reg, uint8_t size, uint8_t* pData);
	void (*delay)(int millis);
	void* pHandle;
}mpu6500_comms_t;

mpu6500_comms_t cb;

void read_mpu(uint8_t reg, uint8_t size, uint8_t* data){
	cb.read(cb.pHandle, MPU6500_ADDR, reg, size, data);
}

void write_mpu(uint8_t reg, uint8_t size, uint8_t* data){
	cb.write(cb.pHandle, MPU6500_ADDR, reg, size, data);
}

int16_t flip_bytes(int16_t bt){
	uint8_t* pByte = (uint8_t*)&bt;
	uint8_t temp = pByte[1];
	pByte[1] = pByte[0];
	pByte[0] = temp;
	return bt; 
}

int mpu6500_init(void (*read_cb)(void* pHandle, uint8_t addr, uint8_t reg, uint8_t size, uint8_t* pData), 
				 void (*write_cb)(void* pHandle, uint8_t addr, uint8_t reg, uint8_t size, uint8_t* pData),
				 void (*delay_cb)(int millis),
				 void* pHandle){
	uint8_t read_buffer[4];
	uint8_t write_buffer[4];
	cb.read = read_cb;
	cb.write = write_cb;
	cb.delay = delay_cb;
	cb.pHandle = pHandle;



	read_mpu(MPU6500_WHO_AM_I, 1, read_buffer);
	if(read_buffer[0] != MPU6500_WHO_AM_I_ID) return -1;

	write_buffer[0] = MPU6500_DEVICE_RESET;
	write_mpu(MPU6500_PWR_MGMT_1, 1, write_buffer);

	cb.delay(100);

	while(read_buffer[0] & MPU6500_DEVICE_RESET) read_mpu(MPU6500_PWR_MGMT_1, 1, read_buffer);

	cb.delay(100);

	write_buffer[0] = MPU6500_TEMP_DIS | 0x01; //select PLL
	write_mpu(MPU6500_PWR_MGMT_1, 1, write_buffer);

	write_buffer[0] = 0x00;
	write_mpu(MPU6500_SMPLRT_DIV, 1, write_buffer);

	write_buffer[0] = MPU6500_G_DLPF_92HZ;
	write_mpu(MPU6500_CONFIG, 1, write_buffer);

	write_buffer[0] = MPU6500_GYRO_FS_2000DPS;
	write_mpu(MPU6500_GYRO_CONFIG, 1, write_buffer);

	write_buffer[0] = MPU6500_ACCEL_FS_8G;
	write_mpu(MPU6500_ACCEL_CONFIG, 1, write_buffer);
	
	write_buffer[0] = MPU6500_A_DPLF_92HZ;
	write_mpu(MPU6500_ACCEL_CONFIG2, 1, write_buffer);

	write_buffer[0] = MPU6500_LATCH_INT_EN | MPU6500_INT_ANYRD_2CLEAR | MPU6500_INT_BYPASS_EN;
	write_mpu(MPU6500_INT_PIN_CFG, 1, write_buffer);

	write_buffer[0] = MPU6500_INT_RAW_RDY_EN;
	write_mpu(MPU6500_INT_ENABLE, 1, write_buffer);

	return 1;

}

int mpu6500_getAccel(mpu_vector_t* pAccel){
	mpu_raw_vector_t accel;
	read_mpu(MPU6500_ACCEL_XOUT_H, 6, (uint8_t*) &accel);
	pAccel->x = (float) flip_bytes(accel.x);
	pAccel->y = (float) flip_bytes(accel.y);
	pAccel->z = (float) flip_bytes(accel.z);
	return 1;
}

int mpu6500_getGyro(mpu_vector_t* pGyro){
	mpu_raw_vector_t gyro;
	read_mpu(MPU6500_GYRO_XOUT_H, 6, (uint8_t*) &gyro);
	pGyro->x = (float) flip_bytes(gyro.x);
	pGyro->y = (float) flip_bytes(gyro.y);
	pGyro->z = (float) flip_bytes(gyro.z);
	return 1;
}

int mpu6500_getGyroAccel(mpu_vector_t* pGyro, mpu_vector_t* pAccel){
	mpu6500_getAccel(pAccel);
	mpu6500_getGyro(pGyro);
	return 1;
}

