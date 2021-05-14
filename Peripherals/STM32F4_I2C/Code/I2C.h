#ifndef __I2C_H
#define __I2C_H

#include <stdint.h>
#include "stm32f4xx.h" // Connect lib for your stm

typedef enum
{
	I2C_READ,
	I2C_WRITE
} I2C_DirectionTransfer;

typedef struct
{
	I2C_TypeDef *i2c;
	uint8_t I2C_addres;
	uint8_t I2C_reg;
	I2C_DirectionTransfer I2C_direct;
	short int I2C_count;
	uint8_t I2C_addrInc;
	uint8_t I2C_MultiByte;
	uint8_t I2C_FlagStart;
	uint8_t *I2C_Rx;
	uint8_t I2C_Tx;
	uint8_t I2C_FlagReady;
	uint8_t I2C_DebagPoint;
} I2C_StructBehavior;

void I2C_EventHandler(I2C_StructBehavior *bstruct);
void I2C_SetI2C(I2C_StructBehavior *bstruct, I2C_TypeDef *i2c);
void I2C_ParamReset(I2C_StructBehavior *bstruct);
void I2C_SetParam(I2C_StructBehavior *bstruct, I2C_DirectionTransfer dir, uint8_t addr,
					uint8_t reg, uint8_t *rxBuf, uint8_t txBuf, short int nbrData);

#endif


























