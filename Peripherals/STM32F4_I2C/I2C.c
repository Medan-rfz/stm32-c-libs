#include "I2C.h"

//#define I2C_DEBUG

typedef enum
{
	I2C_START_NO_GENERATED,
	I2C_START_GENERATED,
	I2C_RESTART_GENERATED
} I2C_StatTransfer;

void I2C_WriteSingleByte(I2C_StructBehavior *bstruct);
void I2C_ReadSingleByte(I2C_StructBehavior *bstruct);
void I2C_ReadMultiByte(I2C_StructBehavior *bstruct);
void I2C_SetDebagPoint(I2C_StructBehavior *bstruct, uint8_t nbr);
	
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
__INLINE void I2C_EventHandler(I2C_StructBehavior *bstruct)
{
	// Transfer single byte //
	if(bstruct->I2C_MultiByte == 0)
	{
		// Write single byte //
		if(bstruct->I2C_direct == I2C_WRITE)
		{
			I2C_WriteSingleByte(bstruct);
		}
		// Read single byte //
		else if(bstruct->I2C_direct == I2C_READ)
		{
			I2C_ReadSingleByte(bstruct);
		}
	}
	// Transfer multi byte //
	else if(bstruct->I2C_MultiByte == 1)
	{
		// Read multi byte //
		if(bstruct->I2C_direct == I2C_READ)
		{
			I2C_ReadMultiByte(bstruct);
		}
	}
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
__INLINE void I2C_WriteSingleByte(I2C_StructBehavior *bstruct)
{
	if(READ_BIT(bstruct->i2c->SR1, I2C_SR1_SB) == (I2C_SR1_SB))
			{
				bstruct->i2c->DR = bstruct->I2C_addres & 0xFE;
#ifdef I2C_DEBUG
				I2C_SetDebagPoint(bstruct, 0x01);
#endif
			}
			else if(READ_BIT(bstruct->i2c->SR1, I2C_SR1_ADDR) == (I2C_SR1_ADDR))
			{
				bstruct->i2c->SR2;
				bstruct->i2c->DR = bstruct->I2C_reg;
#ifdef I2C_DEBUG
				I2C_SetDebagPoint(bstruct, 0x02);
#endif
			}
			else if(READ_BIT(bstruct->i2c->SR1, I2C_SR1_TXE) == (I2C_SR1_TXE))
			{
				if(bstruct->I2C_count > 0)
				{
					bstruct->i2c->DR = bstruct->I2C_Tx;
					bstruct->I2C_count--;
#ifdef I2C_DEBUG
					I2C_SetDebagPoint(bstruct, 0x03);
#endif
				}
				else
				{
					bstruct->i2c->CR1	|= I2C_CR1_STOP;
					I2C_ParamReset(bstruct);
#ifdef I2C_DEBUG
					I2C_SetDebagPoint(bstruct, 0x04);
#endif
				}
			}
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
__INLINE void I2C_ReadSingleByte(I2C_StructBehavior *bstruct)
{
	if(READ_BIT(bstruct->i2c->SR1, I2C_SR1_SB) == (I2C_SR1_SB))
		{
			if (bstruct->I2C_FlagStart == I2C_RESTART_GENERATED)
			{
				bstruct->i2c->DR = bstruct->I2C_addres | 0x01;
#ifdef I2C_DEBUG
				I2C_SetDebagPoint(bstruct, 0x14);
#endif
			}
			else if(bstruct->I2C_FlagStart == I2C_START_NO_GENERATED)
			{
				bstruct->i2c->DR = bstruct->I2C_addres & 0xFE;
				bstruct->I2C_FlagStart = I2C_START_GENERATED;
#ifdef I2C_DEBUG
				I2C_SetDebagPoint(bstruct, 0x11);
#endif
			}
		}
		else if(READ_BIT(bstruct->i2c->SR1, I2C_SR1_ADDR) == (I2C_SR1_ADDR))
		{
			if (bstruct->I2C_FlagStart == I2C_START_GENERATED)
			{
				bstruct->i2c->SR2;
				bstruct->i2c->DR = bstruct->I2C_reg;
#ifdef I2C_DEBUG
				I2C_SetDebagPoint(bstruct, 0x12);
#endif
			}
			else if(bstruct->I2C_FlagStart == I2C_RESTART_GENERATED)
			{
				bstruct->i2c->SR2;
				bstruct->i2c->CR1	|= I2C_CR1_STOP;
				bstruct->i2c->CR1	&= ~I2C_CR1_ACK;
#ifdef I2C_DEBUG
				I2C_SetDebagPoint(bstruct, 0x15);
#endif
			}
		}
		else if((READ_BIT(bstruct->i2c->SR1, I2C_SR1_TXE) == (I2C_SR1_TXE)) && (bstruct->I2C_FlagStart == I2C_START_GENERATED))
		{
			bstruct->i2c->CR1 |= I2C_CR1_START;
			bstruct->I2C_FlagStart = I2C_RESTART_GENERATED;
#ifdef I2C_DEBUG
			I2C_SetDebagPoint(bstruct, 0x13);
#endif
		}
		else if(READ_BIT(bstruct->i2c->SR1, I2C_SR1_RXNE) == (I2C_SR1_RXNE))
		{
			*(bstruct->I2C_Rx) = (uint8_t)bstruct->i2c->DR;
			I2C_ParamReset(bstruct);
#ifdef I2C_DEBUG
			I2C_SetDebagPoint(bstruct, 0x16);
#endif
		}
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
__INLINE void I2C_ReadMultiByte(I2C_StructBehavior *bstruct)
{
	if(READ_BIT(bstruct->i2c->SR1, I2C_SR1_SB) == (I2C_SR1_SB))
	{
		if (bstruct->I2C_FlagStart == I2C_RESTART_GENERATED)
		{
			bstruct->i2c->DR = bstruct->I2C_addres | 0x01;
		}
		else if(bstruct->I2C_FlagStart == I2C_START_NO_GENERATED)
		{
			bstruct->i2c->DR = bstruct->I2C_addres & 0xFE;
			bstruct->I2C_FlagStart = I2C_START_GENERATED;
#ifdef I2C_DEBUG
			I2C_SetDebagPoint(bstruct, 0x21);
#endif
		}
	}
	else if(READ_BIT(bstruct->i2c->SR1, I2C_SR1_ADDR) == (I2C_SR1_ADDR))
	{
		if (bstruct->I2C_FlagStart == I2C_START_GENERATED)
		{
			bstruct->i2c->SR2;
			bstruct->i2c->DR = bstruct->I2C_reg | 0x80;
#ifdef I2C_DEBUG
			I2C_SetDebagPoint(bstruct, 0x22);
#endif
		}
		else if(bstruct->I2C_FlagStart == I2C_RESTART_GENERATED)
		{
			bstruct->i2c->SR2;
			bstruct->i2c->CR1	|= I2C_CR1_ACK;
			bstruct->i2c->CR1	&= ~I2C_CR1_STOP;
#ifdef I2C_DEBUG
			I2C_SetDebagPoint(bstruct, 0x24);
#endif
		}
	}
	else if((READ_BIT(bstruct->i2c->SR1, I2C_SR1_TXE) == (I2C_SR1_TXE)) && (bstruct->I2C_FlagStart == I2C_START_GENERATED))
	{
		bstruct->i2c->CR1 |= I2C_CR1_START;
		bstruct->I2C_FlagStart = I2C_RESTART_GENERATED;
#ifdef I2C_DEBUG
		I2C_SetDebagPoint(bstruct, 0x23);
#endif
	}
	else if(READ_BIT(bstruct->i2c->SR1, I2C_SR1_RXNE) == (I2C_SR1_RXNE))
	{
		if(bstruct->I2C_count > 1)
		{
			*(bstruct->I2C_Rx) = (uint8_t)bstruct->i2c->DR;
			bstruct->I2C_Rx++;
			bstruct->I2C_count--;
#ifdef I2C_DEBUG
			I2C_SetDebagPoint(bstruct, 0x25);
#endif

			if(bstruct->I2C_count == 1)
			{
				bstruct->i2c->CR1	&= ~I2C_CR1_ACK;
				bstruct->i2c->CR1	|= I2C_CR1_STOP;
#ifdef I2C_DEBUG
				I2C_SetDebagPoint(bstruct, 0x26);
#endif
			}
		}
		else if(bstruct->I2C_count == 1)
		{
			*(bstruct->I2C_Rx) = (uint8_t)bstruct->i2c->DR;
			I2C_ParamReset(bstruct);
#ifdef I2C_DEBUG
			I2C_SetDebagPoint(bstruct, 0x27);
#endif
		}
	}
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void I2C_SetI2C(I2C_StructBehavior *bstruct, I2C_TypeDef *i2c)
{
	bstruct->i2c = i2c;
	bstruct->I2C_FlagReady = 1;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void I2C_SetParam(I2C_StructBehavior *bstruct, I2C_DirectionTransfer dir, uint8_t addr,
					uint8_t reg, uint8_t *rxBuf, uint8_t txBuf, short int nbrData)
{
	bstruct->I2C_direct 	= dir;
	bstruct->I2C_addres 	= addr;
	bstruct->I2C_reg 	= reg;
	bstruct->I2C_Rx		= rxBuf;
	bstruct->I2C_Tx		= txBuf;
	bstruct->I2C_count	= nbrData;
	bstruct->I2C_FlagReady = 0;

	if(nbrData > 1)
		bstruct->I2C_MultiByte = 1;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void I2C_ParamReset(I2C_StructBehavior *bstruct)
{
	bstruct->I2C_addres 	= 0;
	bstruct->I2C_reg 	= 0;
	bstruct->I2C_Rx		= 0;
	bstruct->I2C_Tx		= 0;
	bstruct->I2C_direct	= 0;
	bstruct->I2C_count 	= 0;
	bstruct->I2C_addrInc = 0;
	bstruct->I2C_MultiByte 	= 0;
	bstruct->I2C_FlagStart 	= 0;
	bstruct->I2C_FlagReady 	= 1;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void I2C_SetDebagPoint(I2C_StructBehavior *bstruct, uint8_t nbr)
{
	bstruct->I2C_DebagPoint = nbr;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//



























