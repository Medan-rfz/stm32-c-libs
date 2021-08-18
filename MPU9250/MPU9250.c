#include "MPU9250.h"

MPU9250_Status MPU9250_WaitEndTransfer(MPU9250_StructBehavior *bstruct, uint32_t timeout);

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
MPU9250_Status MPU9250_Init(MPU9250_StructBehavior *bstruct, I2C_TypeDef *i2c)
{
	I2C_SetI2C(&(bstruct->i2c), i2c);
	I2C_ParamReset(&(bstruct->i2c));

	bstruct->Addr = 0x68U;
	bstruct->AK893Addr = 0x0CU;

	uint8_t data = MPU9250_ReadOnceReg(bstruct, 117);

	if(data != 0x71)
	{
		bstruct->Stat = MPU9250_NotFound;
		return MPU9250_NotFound;
	}

	MPU9250_WriteReg(bstruct, MPU9250_INT_PIN_CFG, 0x02);


	bstruct->Stat = MPU9250_Ok;
	return MPU9250_Ok;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
uint8_t MPU9250_ReadOnceReg(MPU9250_StructBehavior *bstruct, uint8_t ReadAddr)
{
	uint8_t data = 0;
	I2C_SetParam(&bstruct->i2c, I2C_READ, bstruct->Addr, ReadAddr, &data, 0, 1);
	I2C_Start(&bstruct->i2c);

	MPU9250_WaitEndTransfer(bstruct, 100000);

	return data;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void MPU9250_ReadReg(MPU9250_StructBehavior *bstruct, uint8_t ReadAddr, uint8_t *outBuf, short int count)
{
	I2C_SetParam(&bstruct->i2c, I2C_READ, bstruct->Addr, ReadAddr, outBuf, 0, count);
	I2C_Start(&bstruct->i2c);

	MPU9250_WaitEndTransfer(bstruct, 100000);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void MPU9250_WriteReg(MPU9250_StructBehavior *bstruct, uint8_t WriteAddr, uint8_t data)
{
	I2C_SetParam(&bstruct->i2c, I2C_WRITE, bstruct->Addr, WriteAddr, 0, data, 1);
	I2C_Start(&bstruct->i2c);

	MPU9250_WaitEndTransfer(bstruct, 100000);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
uint8_t MPU9250_ReadOnceRegAK893(MPU9250_StructBehavior *bstruct, uint8_t ReadAddr)
{
	uint8_t data = 0;
	I2C_SetParam(&bstruct->i2c, I2C_READ, bstruct->AK893Addr, ReadAddr, &data, 0, 1);
	I2C_Start(&bstruct->i2c);

	MPU9250_WaitEndTransfer(bstruct, 100000);

	return data;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void MPU9250_ReadRegAK893(MPU9250_StructBehavior *bstruct, uint8_t ReadAddr, uint8_t *outBuf, short int count)
{
	I2C_SetParam(&bstruct->i2c, I2C_READ, bstruct->AK893Addr, ReadAddr, outBuf, 0, count);
	I2C_Start(&bstruct->i2c);

	MPU9250_WaitEndTransfer(bstruct, 100000);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void MPU9250_WriteRegAK893(MPU9250_StructBehavior *bstruct, uint8_t WriteAddr, uint8_t data)
{
	I2C_SetParam(&bstruct->i2c, I2C_WRITE, bstruct->AK893Addr, WriteAddr, 0, data, 1);
	I2C_Start(&bstruct->i2c);

	MPU9250_WaitEndTransfer(bstruct, 100000);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void MPU9250_ReadAllMeasurements(MPU9250_StructBehavior *bstruct, uint16_t *accel, uint16_t *gyro, uint16_t *mag, uint16_t *temp)
{
	uint8_t MPU9250_data[14] = { 0 };
	uint8_t AK893_data[6] = { 0 };

	MPU9250_ReadReg(bstruct, MPU9250_ACCEL_XOUT_H, MPU9250_data, 14);
	MPU9250_ReadRegAK893(bstruct, AK8963_XOUT_L, AK893_data, 6);

	if(accel != 0)
	{
		accel[0] = ((uint16_t)MPU9250_data[0] << 8) | ((uint16_t)MPU9250_data[1]);
		accel[1] = ((uint16_t)MPU9250_data[2] << 8) | ((uint16_t)MPU9250_data[3]);
		accel[2] = ((uint16_t)MPU9250_data[4] << 8) | ((uint16_t)MPU9250_data[5]);
	}

	if(temp != 0)
	{
		*temp = ((uint16_t)MPU9250_data[6] << 8) | ((uint16_t)MPU9250_data[7]);
	}

	if(gyro != 0)
	{
		gyro[0] = ((uint16_t)MPU9250_data[8] << 8) | ((uint16_t)MPU9250_data[9]);
		gyro[1] = ((uint16_t)MPU9250_data[10] << 8) | ((uint16_t)MPU9250_data[11]);
		gyro[2] = ((uint16_t)MPU9250_data[12] << 8) | ((uint16_t)MPU9250_data[13]);
	}

	if(mag != 0)
	{
		mag[0] = ((uint16_t)AK893_data[1] << 8) | ((uint16_t)AK893_data[0]);
		mag[1] = ((uint16_t)AK893_data[3] << 8) | ((uint16_t)AK893_data[2]);
		mag[2] = ((uint16_t)AK893_data[5] << 8) | ((uint16_t)AK893_data[4]);
	}
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
MPU9250_Status MPU9250_WaitEndTransfer(MPU9250_StructBehavior *bstruct, uint32_t timeout)
{
	while(!bstruct->i2c.I2C_FlagReady)
	{
		if((bstruct->i2c.i2c->SR1 & I2C_SR1_AF) || ((--timeout) == 0))
			return MPU9250_i2cTimeout;
	}

	return MPU9250_Ok;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//



