#include "stm_pch.h"	// User precompile file in project folder with header on used stm32
#include "SPI.h"
#include "NRF24L01.h"
#include "Delay.h"

#define NRF24L01_MAX_PAYLOAD 32

#define CSN_SELECT 		bstruct->Port->BSRR = (1 << (bstruct->PinCSN + 16))
#define CSN_UNSELECT 	bstruct->Port->BSRR = (1 << bstruct->PinCSN)

#define CE_ENABLE 		bstruct->Port->BSRR = (1 << bstruct->PinCE)
#define CE_DISABLE 		bstruct->Port->BSRR = (1 << (bstruct->PinCE + 16))

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
/* Functions of initialization given as example */
void NRF24L01_InitTx(NRF24L01_TypeDef *bstruct)
{
	CE_DISABLE;
	DelayUs(5000);
	NRF24L01_WriteReg(bstruct, NRF24L01_CONFIG, 0x00);
	DelayUs(2500);
	/* Enable pipe 0 and ACK */
	NRF24L01_WriteReg(bstruct, NRF24L01_EN_AA, 0x01);
	NRF24L01_WriteReg(bstruct, NRF24L01_EN_RXADDR, 0x01);
	/* Addr 5 bytes */
	NRF24L01_WriteReg(bstruct, NRF24L01_SETUP_AW, 0x03);
	/* Retransmit 1500 us, 15 packets maximum */
	NRF24L01_WriteReg(bstruct, NRF24L01_SETUP_RETR, 0x5F);
	NRF24L01_WriteReg(bstruct, NRF24L01_FEATURE, 0);
	NRF24L01_WriteReg(bstruct, NRF24L01_DYNPD, 0);
	NRF24L01_WriteReg(bstruct, NRF24L01_STATUS, 0x70);
	NRF24L01_WriteReg(bstruct, NRF24L01_RF_CH, bstruct->FreqCh);
	/* Power 0 dBm, Rate 1 Mbps */
	NRF24L01_WriteReg(bstruct, NRF24L01_RF_SETUP, 0x06);
	NRF24L01_WriteReg(bstruct, NRF24L01_RX_PW_P0, bstruct->PayloadLen);

	NRF24L01_WriteBuf(bstruct, NRF24L01_TX_ADDR, bstruct->SelfAddr, 5);
	NRF24L01_WriteBuf(bstruct, NRF24L01_RX_ADDR_P0, bstruct->SelfAddr, 5);

	/* Power up, CRC 1 byte enable, RX mode */
	NRF24L01_WriteReg(bstruct, NRF24L01_CONFIG, 0x0a);
	DelayUs(2500);
	NRF24L01_FlushRX(bstruct);
	NRF24L01_FlushTX(bstruct);

	bstruct->TxDone = true;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void NRF24L01_InitRx(NRF24L01_TypeDef *bstruct)
{
	CE_DISABLE;
	DelayUs(5000);
	NRF24L01_WriteReg(bstruct, NRF24L01_CONFIG, 0x00);
	DelayUs(2500);
	NRF24L01_WriteReg(bstruct, NRF24L01_EN_AA, 0x01);
	NRF24L01_WriteReg(bstruct, NRF24L01_EN_RXADDR, 0x01);
	NRF24L01_WriteReg(bstruct, NRF24L01_SETUP_AW, 0x03);
	NRF24L01_WriteReg(bstruct, NRF24L01_SETUP_RETR, 0x5F);
	NRF24L01_WriteReg(bstruct, NRF24L01_FEATURE, 0);
	NRF24L01_WriteReg(bstruct, NRF24L01_DYNPD, 0);
	NRF24L01_WriteReg(bstruct, NRF24L01_STATUS, 0x70);
	NRF24L01_WriteReg(bstruct, NRF24L01_RF_CH, bstruct->FreqCh);
	NRF24L01_WriteReg(bstruct, NRF24L01_RF_SETUP, 0x06);
	NRF24L01_WriteReg(bstruct, NRF24L01_RX_PW_P0, bstruct->PayloadLen);

	NRF24L01_WriteBuf(bstruct, NRF24L01_RX_ADDR_P1, bstruct->RemoteAddr, 5);

	NRF24L01_WriteReg(bstruct, NRF24L01_CONFIG, 0x0b);
	DelayUs(2500);
	NRF24L01_FlushRX(bstruct);
	NRF24L01_FlushTX(bstruct);

	CE_ENABLE;
	DelayUs(150);

	bstruct->TxDone = true;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
uint8_t NRF24L01_ReadReg(NRF24L01_TypeDef *bstruct, uint8_t reg)
{
	uint8_t data = 0;

	CSN_SELECT;
	SPI_TransferByte(bstruct->Spi, reg, 100000);
	data = SPI_TransferByte(bstruct->Spi, 0xFF, 100000);
	CSN_UNSELECT;

	return data;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void NRF24L01_WriteReg(NRF24L01_TypeDef *bstruct, uint8_t reg, uint8_t data)
{
	CSN_SELECT;
	SPI_TransferByte(bstruct->Spi, reg | 0x20, 100000);
	SPI_TransferByte(bstruct->Spi, data, 100000);
	CSN_UNSELECT;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void NRF24L01_ReadBuf(NRF24L01_TypeDef *bstruct, uint8_t reg, uint8_t *pBuf, uint8_t size)
{
	CSN_SELECT;
	SPI_TransferByte(bstruct->Spi, reg, 100000);

	for(int i = 0; i < size; i++)
		pBuf[i] = SPI_TransferByte(bstruct->Spi, 0xFF, 100000);

	CSN_UNSELECT;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void NRF24L01_WriteBuf(NRF24L01_TypeDef *bstruct, uint8_t reg, uint8_t *pBuf, uint8_t size)
{
	CSN_SELECT;
	SPI_TransferByte(bstruct->Spi, reg | 0x20, 100000);

	for(int i = 0; i < size; i++)
		SPI_TransferByte(bstruct->Spi, pBuf[i], 100000);

	CSN_UNSELECT;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void NRF24L01_FlushRX(NRF24L01_TypeDef *bstruct)
{
	CSN_SELECT;
	SPI_TransferByte(bstruct->Spi, NRF24L01_FLUSH_RX, 100000);
	CSN_UNSELECT;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void NRF24L01_FlushTX(NRF24L01_TypeDef *bstruct)
{
	CSN_SELECT;
	SPI_TransferByte(bstruct->Spi, NRF24L01_FLUSH_TX, 100000);
	CSN_UNSELECT;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void NRF24L01_ModeRX(NRF24L01_TypeDef *bstruct)
{
	uint8_t tmpReg = NRF24L01_ReadReg(bstruct, NRF24L01_CONFIG);
	tmpReg |= NRF24L01_CONFIG_PWR_UP | NRF24L01_CONFIG_PRIM_RX;
	NRF24L01_WriteReg(bstruct, NRF24L01_CONFIG, tmpReg);
	CE_ENABLE;
	DelayUs(150);

	NRF24L01_FlushRX(bstruct);
	NRF24L01_FlushTX(bstruct);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void NRF24L01_ModeTX(NRF24L01_TypeDef *bstruct)
{
	CE_DISABLE;
	uint8_t tmpReg = NRF24L01_ReadReg(bstruct, NRF24L01_CONFIG);
	tmpReg &= ~NRF24L01_CONFIG_PRIM_RX;
	tmpReg |= NRF24L01_CONFIG_PWR_UP;
	NRF24L01_WriteReg(bstruct, NRF24L01_CONFIG, tmpReg);
	DelayUs(150);

	NRF24L01_FlushRX(bstruct);
	NRF24L01_FlushTX(bstruct);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
int NRF24L01_Transmit(NRF24L01_TypeDef *bstruct, uint8_t *pBuf, uint8_t size)
{
	if(!bstruct->TxDone)
		return -1;

	bstruct->TxDone = false;

	NRF24L01_ModeTX(bstruct);
	NRF24L01_WriteBuf(bstruct, NRF24L01_WR_TX_PLOAD, pBuf, size);
	CE_ENABLE;
	DelayUs(15);
	CE_DISABLE;

	return 0;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
void NRF24L01_CallbackIRQ(NRF24L01_TypeDef *bstruct)
{
	uint8_t stat = NRF24L01_ReadReg(bstruct, NRF24L01_STATUS);

	if(stat & NRF24L01_STATUS_RX_DS)
	{
		uint8_t tmp[NRF24L01_MAX_PAYLOAD] = { 0 };
		NRF24L01_ReadBuf(bstruct, NRF24L01_RD_RX_PLOAD, tmp, bstruct->PayloadLen); // XXX Check buffer behavior
		if(bstruct->RxCallback)
			bstruct->RxCallback(tmp, bstruct->PayloadLen);

		NRF24L01_WriteReg(bstruct, NRF24L01_STATUS, NRF24L01_STATUS_RX_DS);
	}

	if(stat & NRF24L01_STATUS_TX_DS)
	{
		bstruct->TxDone = true;
		NRF24L01_WriteReg(bstruct, NRF24L01_STATUS, NRF24L01_STATUS_TX_DS);
	}

	if(stat & NRF24L01_STATUS_MAX_RT)
	{
		// TODO All packets lost
		bstruct->TxDone = true;
		NRF24L01_FlushTX(bstruct);
		NRF24L01_WriteReg(bstruct, NRF24L01_STATUS, NRF24L01_STATUS_MAX_RT);
	}
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
