#ifndef __MFRC522_H
#define __MFRC522_H

#include <stdint.h>
#include "SPI.h"

typedef enum
{
	MFRC522_statOK,
	MFRC522_statErr,
	MFRC522_timeOut
} MFRC522_Stat;

typedef enum
{
	MFRC522_Idle 		= 0x00,
	MFRC522_Authent		= 0x0E,
	MFRC522_Receive		= 0x08,
	MFRC522_Transmit	= 0x04,
	MFRC522_Transceive	= 0x0C,
	MFRC522_ResetPhase	= 0x0F,
	MFRC522_CalacCRC	= 0x03
} MFRC522_Command;

typedef enum
{
	MFRC522_ReqId 			= 0x26,
	MFRC522_WakeUp			= 0x52,
	MFRC522_AntiCollCL1		= 0x9320,
	MFRC522_SelectCL1		= 0x9370,
	MFRC522_AntiCollCL2		= 0x9520,
	MFRC522_SelectCL2		= 0x9570,
	MFRC522_Halt			= 0x50,
	MFRC522_AuthentA		= 0x60,
	MFRC522_AuthentB		= 0x61,
	MFRC522_Read			= 0x30,
	MFRC522_Write			= 0xA0,
	MFRC522_Decrement		= 0xC0,
	MFRC522_Increment		= 0xC1,
	MFRC522_Restore			= 0xC2,
	MFRC522_Transfer		= 0xB0
} MFRC522_MifareCommand;

#define MFRC522_CommandReg 			0x01
#define MFRC522_ComIEnReg 			0x02
#define MFRC522_DivIEnReg 			0x03
#define MFRC522_ComIrqReg 			0x04
#define MFRC522_DivIrqReg 			0x05
#define MFRC522_ErrorReg 			0x06
#define MFRC522_Status1Reg 			0x07
#define MFRC522_Status2Reg 			0x08
#define MFRC522_FIFODataReg 		0x09
#define MFRC522_FIFOLevelReg 		0x0A
#define MFRC522_WaterLevelReg 		0x0B
#define MFRC522_ControlReg 			0x0C
#define MFRC522_BitFramingReg 		0x0D
#define MFRC522_CollReg 			0x0E

#define MFRC522_ModeReg 			0x11
#define MFRC522_TxModeReg 			0x12
#define MFRC522_RxModeReg 			0x13
#define MFRC522_TxControlReg		0x14
#define MFRC522_TxASKReg 			0x15
#define MFRC522_TxSelReg 			0x16
#define MFRC522_RxSelReg 			0x17
#define MFRC522_RxThresholdReg 		0x18
#define MFRC522_DemodReg 			0x19
#define MFRC522_MfTxReg 			0x1C
#define MFRC522_MfRxReg 			0x1D
#define MFRC522_SerialSpeedReg 		0x1F

#define MFRC522_CRCResultHReg 		0x21
#define MFRC522_CRCResultLReg	 	0x22
#define MFRC522_ModWidthReg 		0x24
#define MFRC522_RFCfgReg 			0x26
#define MFRC522_GsNReg 				0x27
#define MFRC522_CWGsPReg 			0x28
#define MFRC522_ModGsPReg 			0x29
#define MFRC522_TModeReg 			0x2A
#define MFRC522_TPrescalerReg 		0x2B
#define MFRC522_TReloadHReg 		0x2C
#define MFRC522_TReloadLReg 		0x2D
#define MFRC522_TCounterValHReg 	0x2E
#define MFRC522_TCounterValLReg 	0x2F

#define MFRC522_TestSel1Reg 		0x31
#define MFRC522_TestSel2Reg 		0x32
#define MFRC522_TestPinEnReg		0x33
#define MFRC522_TestPinValueReg 	0x34
#define MFRC522_TestBusReg 			0x35
#define MFRC522_AutoTestReg 		0x36
#define MFRC522_VersionReg 			0x37
#define MFRC522_AnalogTestReg 		0x38
#define MFRC522_TestDAC1Reg 		0x39
#define MFRC522_TestDAC2Reg 		0x3A
#define MFRC522_TestADCReg 			0x3B


void MFRC522_Init(SPI_TypeDef *spi);
MFRC522_Stat MFRC522_WriteByte(SPI_TypeDef *spi, uint8_t reg, uint8_t data);
uint8_t MFRC522_ReadByte(SPI_TypeDef *spi, uint8_t reg);
MFRC522_Stat MFRC522_Request(SPI_TypeDef *spi, uint8_t mode, uint8_t *tagType);
MFRC522_Stat MFRC522_AntiCollision(SPI_TypeDef *spi, uint8_t *SN);
uint8_t MFRC522_SelectChip(SPI_TypeDef *spi, uint8_t *SN);
MFRC522_Stat MFRC522_AuthCard(SPI_TypeDef *spi, MFRC522_MifareCommand mode, uint8_t blockAddr, uint8_t *key, uint8_t *SN);
MFRC522_Stat MFRC522_ReadCard(SPI_TypeDef *spi, uint8_t blockAddr, uint8_t *rxBuf);
MFRC522_Stat MFRC522_WriteCard(SPI_TypeDef *spi, uint8_t blockAddr, uint8_t *txBuf);
MFRC522_Stat MFRC522_Helt(SPI_TypeDef *spi);
void MFRC522_test();

#endif

