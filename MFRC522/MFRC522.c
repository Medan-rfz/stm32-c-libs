#include "stm_pch.h"	// User precompile file in project folder with header on used stm32
#include "SPI.h"
#include "MFRC522.h"
#include "Delay.h"

/////////////////////////////////////////////////////////
/////		Settings block
/////////////////////////////////////////////////////////

#define MFRC522_TimeOut_us	100

#define MFRC522_RST_PIN 0U
#define	MFRC522_RST_PORT GPIOB
#define MFRC522_NSS_PIN 1U
#define	MFRC522_NSS_PORT GPIOB

/////////////////////////////////////////////////////////
/////		End settings block
/////////////////////////////////////////////////////////

#define MFRC522_RST_SET MFRC522_RST_PORT->BSRR = (1<<MFRC522_RST_PIN)
#define MFRC522_RST_RESET MFRC522_RST_PORT->BSRR = (1<<(MFRC522_RST_PIN + 16))
#define MFRC522_NSS_SET MFRC522_NSS_PORT->BSRR = (1<<MFRC522_NSS_PIN)
#define MFRC522_NSS_RESET MFRC522_NSS_PORT->BSRR = (1<<(MFRC522_NSS_PIN + 16))

#define MFRC522_MAX_LEN 16


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
MFRC522_Stat MFRC522_WriteByte(SPI_TypeDef *spi, uint8_t reg, uint8_t data)
{
	MFRC522_NSS_RESET;
	SPI_TransferByte(spi, ((reg<<1) & 0x7F), MFRC522_TimeOut_us);
	SPI_TransferByte(spi, data, MFRC522_TimeOut_us);
	MFRC522_NSS_SET;

	return MFRC522_statOK;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
uint8_t MFRC522_ReadByte(SPI_TypeDef *spi, uint8_t reg)
{
	uint8_t rxTmp;

	MFRC522_NSS_RESET;
	SPI_TransferByte(spi, ((reg<<1) | 0x80), MFRC522_TimeOut_us);
	rxTmp = SPI_TransferByte(spi, 0, MFRC522_TimeOut_us);
	MFRC522_NSS_SET;

	return rxTmp;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void MFRC522_SetBitMask(SPI_TypeDef *spi, uint8_t reg, uint8_t msk)
{
	MFRC522_WriteByte(spi, reg, MFRC522_ReadByte(spi, reg) | msk);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void MFRC522_ResetBitMask(SPI_TypeDef *spi, uint8_t reg, uint8_t msk)
{
	MFRC522_WriteByte(spi, reg, MFRC522_ReadByte(spi, reg) & (~msk));
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void MFRC522_Reset()
{
	MFRC522_NSS_SET;
	MFRC522_RST_SET;
	DelayMs(1);
	MFRC522_RST_RESET;
	DelayMs(5);
	MFRC522_RST_SET;
	DelayMs(5);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
__INLINE void MFRC522_Init(SPI_TypeDef *spi)
{
	MFRC522_Reset();

	MFRC522_WriteByte(spi, MFRC522_TxModeReg, 0x00);
	MFRC522_WriteByte(spi, MFRC522_RxModeReg, 0x00);
	MFRC522_WriteByte(spi, MFRC522_ModWidthReg, 0x26);
	MFRC522_WriteByte(spi, MFRC522_TModeReg, 0x80);
	MFRC522_WriteByte(spi, MFRC522_TPrescalerReg, 0xA9);
	MFRC522_WriteByte(spi, MFRC522_TReloadHReg, 0x03);
	MFRC522_WriteByte(spi, MFRC522_TReloadLReg, 0xE8);
	MFRC522_WriteByte(spi, MFRC522_TxASKReg, 0x40);
	MFRC522_WriteByte(spi, MFRC522_ModeReg, 0x3D);

	MFRC522_SetBitMask(spi, MFRC522_TxControlReg, 0x03);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
MFRC522_Stat MFRC522_SessionWithCard(SPI_TypeDef *spi, MFRC522_Command cmd, uint8_t* txBuf,
								uint8_t txLen, uint8_t* rxBuf, uint16_t* rxLen)
{
	__IO int _timeout;
	__IO uint8_t irqMask = 0;

	if(cmd == MFRC522_Authent)
	{
		MFRC522_WriteByte(spi, MFRC522_ComIEnReg, 0x92);
		irqMask = 0x10;		// Irq Idle
	}
	else if(cmd == MFRC522_Transceive)
	{
		MFRC522_WriteByte(spi, MFRC522_ComIEnReg, 0xF7);
		irqMask = 0x30;		// Irq Rx and Idle
	}
	else
		MFRC522_WriteByte(spi, MFRC522_ComIEnReg, 0x80);

	MFRC522_ResetBitMask(spi, MFRC522_ComIrqReg, 0x80);
	MFRC522_SetBitMask(spi, MFRC522_FIFOLevelReg, 0x80);
	MFRC522_WriteByte(spi, MFRC522_CommandReg, MFRC522_Idle);

	for (int i = 0; i < txLen; i++)
		MFRC522_WriteByte(spi, MFRC522_FIFODataReg, txBuf[i]);

	MFRC522_WriteByte(spi, MFRC522_CommandReg, cmd);

	if (cmd == MFRC522_Transceive)
		MFRC522_SetBitMask(spi, MFRC522_BitFramingReg, 0x80);			// Start send

	_timeout = 10000;
	while(!(MFRC522_ReadByte(spi, MFRC522_ComIrqReg) & irqMask))
	{
		DelayUs(1);
		if(--_timeout <= 0) return MFRC522_timeOut;
	}

	MFRC522_ResetBitMask(spi, MFRC522_BitFramingReg, 0x80);

	if(!(MFRC522_ReadByte(spi, MFRC522_ErrorReg) & 0x1B))
	{
		if(cmd == MFRC522_Transceive)
		{
			uint8_t lenFIFO = MFRC522_ReadByte(spi, MFRC522_FIFOLevelReg);
			uint8_t numderLastBit = MFRC522_ReadByte(spi, MFRC522_ControlReg) & 0x07;

			*rxLen = numderLastBit ? ((lenFIFO - 1) * 8 + numderLastBit) : (lenFIFO * 8);

			lenFIFO = (lenFIFO == 0) ? 1 : lenFIFO;
			lenFIFO = (lenFIFO > MFRC522_MAX_LEN) ? MFRC522_MAX_LEN : lenFIFO;

			for(int i = 0; i < lenFIFO; i++)
				rxBuf[i] = MFRC522_ReadByte(spi, MFRC522_FIFODataReg);
		}

		return MFRC522_statOK;
	}
	else
		return MFRC522_statErr;

}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
uint16_t MFRC522_CalcCRC(SPI_TypeDef *spi, uint8_t *inBuf, uint8_t len)
{
	__IO int _timeout;

	MFRC522_ResetBitMask(spi, MFRC522_DivIrqReg, 0x04);
	MFRC522_SetBitMask(spi, MFRC522_FIFOLevelReg, 0x80);

	for(int i = 0; i < len; i++)
		MFRC522_WriteByte(spi, MFRC522_FIFODataReg, inBuf[i]);

	MFRC522_WriteByte(spi, MFRC522_CommandReg, MFRC522_CalacCRC);

	_timeout = 1000;
	while(!(MFRC522_ReadByte(spi, MFRC522_DivIrqReg) & 0x04))
	{
		DelayUs(1);
		if(--_timeout <= 0) break;
	}

	return ((uint16_t)MFRC522_ReadByte(spi, MFRC522_CRCResultHReg) << 8) | (uint16_t)MFRC522_ReadByte(spi, MFRC522_CRCResultLReg);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
MFRC522_Stat MFRC522_Request(SPI_TypeDef *spi, uint8_t mode, uint8_t *tagType)
{
	uint16_t len = 0;

	MFRC522_WriteByte(spi, MFRC522_BitFramingReg, 0x07);

	tagType[0] = mode;

	return MFRC522_SessionWithCard(spi, MFRC522_Transceive, tagType, 1, tagType, &len);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
MFRC522_Stat MFRC522_AntiCollision(SPI_TypeDef *spi, uint8_t *SN)
{
	uint16_t len = 0;

	SN[0] = (uint8_t)(MFRC522_AntiCollCL1>>8);
	SN[1] = (uint8_t)(MFRC522_AntiCollCL1);

	MFRC522_WriteByte(spi, MFRC522_BitFramingReg, 0x00);

	return MFRC522_SessionWithCard(spi, MFRC522_Transceive, SN, 2, SN, &len);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
uint8_t MFRC522_SelectChip(SPI_TypeDef *spi, uint8_t *SN)
{
	uint16_t len = 0;
	uint16_t _CRC;
	uint8_t buf[9];

	MFRC522_WriteByte(spi, MFRC522_BitFramingReg, 0x00);

	buf[0] = (uint8_t)(MFRC522_SelectCL1>>8);
	buf[1] = (uint8_t)(MFRC522_SelectCL1);

	for(int i = 0; i < 5; i++)
		buf[i+2] = SN[i];

	_CRC = MFRC522_CalcCRC(spi, buf, 7);
	buf[7] = (uint8_t)_CRC;
	buf[8] = (uint8_t)(_CRC >> 8);

	if((MFRC522_SessionWithCard(spi, MFRC522_Transceive, buf, 9, buf, &len) == MFRC522_statOK) && (len == 0x18))
		return buf[0];
	else
		return 0;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
MFRC522_Stat MFRC522_AuthCard(SPI_TypeDef *spi, MFRC522_MifareCommand mode, uint8_t blockAddr, uint8_t *key, uint8_t *SN)
{
	uint16_t len = 0;
	uint8_t buf[9];

	MFRC522_WriteByte(spi, MFRC522_BitFramingReg, 0x00);

	buf[0] = mode;
	buf[1] = blockAddr;

	for(int i = 0; i < 6; i++)
		buf[i+2] = key[i];
	for(int i = 0; i < 4; i++)
			buf[i+8] = SN[i];

	if(MFRC522_SessionWithCard(spi, MFRC522_Authent, buf, 12, buf, &len) == MFRC522_statOK)
		if(!(MFRC522_ReadByte(spi, MFRC522_Status2Reg) & 0x08))
			return MFRC522_statErr;
		else
			return MFRC522_statOK;
	else
		return MFRC522_statErr;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
MFRC522_Stat MFRC522_ReadCard(SPI_TypeDef *spi, uint8_t blockAddr, uint8_t *rxBuf)
{
	uint16_t len = 0;
	uint16_t _CRC;

	rxBuf[0] = MFRC522_Read;
	rxBuf[1] = blockAddr;

	_CRC = MFRC522_CalcCRC(spi, rxBuf, 2);
	rxBuf[2] = (uint8_t)_CRC;
	rxBuf[3] = (uint8_t)(_CRC >> 8);

	return MFRC522_SessionWithCard(spi, MFRC522_Transceive, rxBuf, 4, rxBuf, &len);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
MFRC522_Stat MFRC522_WriteCard(SPI_TypeDef *spi, uint8_t blockAddr, uint8_t *txBuf)
{
	uint16_t len = 0;
	uint16_t _CRC;
	uint8_t buf[18];

	buf[0] = MFRC522_Write;
	buf[1] = blockAddr;

	_CRC = MFRC522_CalcCRC(spi, buf, 2);
	buf[2] = (uint8_t)_CRC;
	buf[3] = (uint8_t)(_CRC >> 8);

	if(MFRC522_SessionWithCard(spi, MFRC522_Transceive, buf, 4, buf, &len) != MFRC522_statOK)
		return MFRC522_statErr;

	if((buf[0] & 0x0F) == 0x0A)
	{
		for(int i = 0; i < 16; i++)
			buf[i] = txBuf[i];

		_CRC = MFRC522_CalcCRC(spi, buf, 16);
		buf[16] = (uint8_t)_CRC;
		buf[17] = (uint8_t)(_CRC >> 8);

		if(MFRC522_SessionWithCard(spi, MFRC522_Transceive, buf, 18, buf, &len) == MFRC522_statOK)
			return MFRC522_statOK;
		else
			return MFRC522_statErr;
	}
	else
		return MFRC522_statErr;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
MFRC522_Stat MFRC522_Helt(SPI_TypeDef *spi)
{
	uint16_t len = 0;
	uint8_t buf[4];
	uint16_t _CRC;

	buf[0] = MFRC522_Halt;
	buf[1] = 0;

	_CRC = MFRC522_CalcCRC(spi, buf, 2);
	buf[2] = (uint8_t)_CRC;
	buf[3] = (uint8_t)(_CRC >> 8);

	return MFRC522_SessionWithCard(spi, MFRC522_Transceive, buf, 4, buf, &len);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void MFRC522_test()
{
	MFRC522_Reset();

	uint8_t test[10];

	for(int i = 0; i<10; i++)
		test[i] = 0;

	MFRC522_Stat stat;

	stat = MFRC522_Request(SPI1, MFRC522_ReqId, test);
	stat = MFRC522_AntiCollision(SPI1, test);
}





